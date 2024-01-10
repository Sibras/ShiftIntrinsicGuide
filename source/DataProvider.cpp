/**
 * Copyright Matthew Oliver
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "DataProvider.h"

#include "Application.h"
#include "Downloader.h"

#include <QDateTime>

constexpr uint32_t fileVersion = 0x011000;
constexpr uint32_t fileID = 0xA654BE39;

DataProvider::DataProvider(Application* parent) noexcept
    : parentApp(parent)
{}

void DataProvider::setProgress(const float value) const noexcept
{
    const auto progress2 = progress + std::clamp(value, 0.0F, 1.0F) * progressModifier;
    parentApp->setProgress(progress2);
}

void DataProvider::addProgress(const float value) noexcept
{
    progress += value * progressModifier;
    parentApp->setProgress(progress);
}

InternalData& DataProvider::getData() noexcept
{
    return data;
}

bool DataProvider::setup() noexcept
{
    bool success = true;
    // Load any existing cache from disk
    if (!load()) {
        // If no cache found then create new one
        success = create();
        if (success) {
            // Backup created cache to disk
            (void)store();
        }
    }

    if (success) {
        // Remove loading progress from parent
        parentApp->setProgress(1.0F);
    }
    return success;
}

void DataProvider::clear()
{
    data.instructions.clear();
    data.allTechnologies.clear();
    data.allTypes.clear();
    data.allCategories.clear();
    data.version.clear();
}

bool DataProvider::load() noexcept
{
    if (QFile fileCache("./dataCache"); fileCache.exists() && fileCache.open(QIODevice::ReadOnly)) {
        // Reset UI values to default
        progressModifier = 1.0F;
        progress = 0.0F;
        parentApp->setLoadingTitle("Loading...");
        parentApp->setProgress(0.0F);

        // Load data from cache
        parentApp->setLoadingTitle("Loading data from cache...");
        QDataStream in(&fileCache);
        uint32_t check = 0;
        in >> check;
        if (check != fileID) {
            qWarning() << "File load had invalid identifier";
            return false;
        }
        in >> check;
        if (check != fileVersion) {
            qInfo() << "File version to old, creating new one";
            return false;
        }
        in.setVersion(QDataStream::Qt_6_2);
        in >> data.date;
        if (QDateTime(data.date, QTime::currentTime()).daysTo(QDateTime::currentDateTime()) > 180) {
            qInfo() << "File data is to old, creating new one";
            return false;
        }
        in >> data;
        addProgress(1.0F);
        return true;
    }
    return false;
}

bool DataProvider::store() noexcept
{
    // Stream data model to disk
    if (QFile fileCache("./dataCache"); fileCache.open(QIODevice::WriteOnly)) {
        parentApp->setLoadingTitle("Writing data store to disk...");
        // Store data from cache
        QDataStream out(&fileCache);
        out << fileID;
        out << fileVersion;
        out.setVersion(QDataStream::Qt_6_2);
        out << data.date;
        out << data;
        addProgress(1.0F);
        return true;
    }
    return false;
}

bool DataProvider::create() noexcept
{
    // Reset UI values to default
    progressModifier = 1.0F / 8.0F;
    progress = 0.0F;
    parentApp->setLoadingTitle("Creating...");
    parentApp->setProgress(0.0F);

    QDomDocument dataXMLIntel, dataXMLOps;
    QString intrinFile = "./intrin.xml";
    QString uopsFile = "./uops.xml";
    if (!downloadCache(intrinFile, "Intel Intrinsic Guide", dataXMLIntel,
            QUrl("https://www.intel.com/content/dam/develop/public/us/en/include/intrinsics-guide/data-latest.xml"))) {
        return false;
    }
    if (!downloadCache(uopsFile, "uops.info", dataXMLOps, QUrl("https://www.uops.info/instructions.xml"))) {
        return false;
    }

    parentApp->setLoadingTitle("Creating data store...");
    // Scan through intrinsic data for each function
    QDomElement root = dataXMLIntel.documentElement();
    QDomElement root2 = dataXMLOps.documentElement();
    data.version = root.attribute("version", "3.6.7");
    data.date = QDate::fromString(root.attribute("date", "07/12/2023"), "MM/dd/yyyy");
#ifdef _DEBUG
    if (!data.date.isValid()) {
        qDebug() << "Invalid source date detected for data:" << root.attribute("date", "06/30/2021");
    }
#endif

    const QMap<QString, QString> typesPretty = {{"BF16", "BFloat16"}, {"FP16", "Float16 (half)"},
        {"FP32", "Float32 (float)"}, {"FP64", "Float64 (double)"}, {"MASK", "Mask"},
        {"SI16", "Integer Signed 16 (int16)"}, {"SI32", "Integer Signed 32 (int32)"},
        {"SI64", "Integer Signed 64 (int64)"}, {"SI8", "Integer Signed 8 (int8)"},
        {"UI16", "Integer Unsigned 16 (uint16)"}, {"UI32", "Integer Unsigned 32 (uint32)"},
        {"UI64", "Integer Unsigned 64 (uint64)"}, {"UI8", "Integer Unsigned 8 (uint8)"}};

    const QMap<QString, QString> archsPretty = {{"CON", "Conroe"}, {"WOL", "Wolfdale"}, {"NHM", "Nehalem"},
        {"WSM", "Westmere"}, {"SNB", "Sandy Bridge"}, {"IVB", "Ivy Bridge"}, {"HSW", "Haswell"}, {"BDW", "Broadwell"},
        {"SKL", "Skylake"}, {"SKX", "Skylake-X"}, {"KBL", "Kaby Lake"}, {"CFL", "Coffee Lake"}, {"CNL", "Cannon Lake"},
        {"CLX", "Cascade Lake"}, {"ICL", "Ice Lake"}, {"TGL", "Tiger Lake"}, {"RKL", "Rocket Lake"}, {"ZEN+", "Zen+"},
        {"ZEN2", "Zen2"}, {"ZEN3", "Zen3"}, {"ZEN4", "Zen4"}, {"ADL-P", "Alder Lake (P-Core)"},
        {"ADL-E", "Alder Lake (E-Core)"}, {"BNL", "Bonnell"}, {"AMT", "Airmont"}, {"GLM", "Goldmont"},
        {"GLP", "Goldmont+"}, {"TRM", "Tremont"}};

    const QMap<QString, QString> xedConversions = {{"MASKMOVDQU_XMMdq_XMMdq", "MASKMOVDQU_XMMxub_XMMxub"},
        {"MOVLPS_MEMq_XMMps", "MOVLPS_MEMq_XMMq"}, {"MOVQ_XMMdq_MEMq_0F6E", "MOVQ_XMMdq_MEMq_0F7E"},
        {"MOVQ_MEMq_XMMq_0F7E", "MOVQ_MEMq_XMMq_0FD6"}};

    QList<Instruction> instructions;

    // Loop through each element and get information
    for (auto i = root.firstChild(); !i.isNull(); i = i.nextSibling()) {
        // Check if the child tag name is an 'intrinsic'
        if (auto node = i.toElement(); !node.isNull() && node.tagName() == "intrinsic") {
            // Get the intrinsic attributes
            // tech: is Technologies selection where cpuids is subsection to tech (when not equal)
            // types: is integer/floating point etc.
            // name: is intrinsic name
            // description: is text description of intrinsic operation
            // operation: is pseudo code of intrinsic operation
            // header: is the header file the intrinsic is declared in.
            QString tech = node.attribute("tech", "Unknown");
            QString name = node.attribute("name");
            QList<QString> types, cpuids, categories, xeds;
            QString description, operation, header, instruction;
            QString returnParam, returnName;
            QList<QPair<QString, QString>> parameters;
            if (name.isEmpty()) {
#ifdef _DEBUG
                qDebug() << "Intrinsic element in xml file did not have name attribute: " + node.toText().data();
#endif
                continue;
            }
            // Get each child nodes data
            for (auto child = node.firstChild(); !child.isNull(); child = child.nextSibling()) {
                if (auto childE = child.toElement(); !childE.isNull()) {
                    // Read Name and value
                    if (childE.tagName() == "type") {
                        // Can have multiple type nodes (e.g. Integer and Flag)
                        if (auto t = childE.firstChild().toText(); !t.isNull()) {
                            types.emplaceBack(t.data());
                        }
                    } else if (childE.tagName() == "CPUID") {
                        // Can have multiple cpuid nodes (e.g. different AVX512 sets)
                        if (auto t = childE.firstChild().toText(); !t.isNull()) {
                            cpuids.emplaceBack(t.data());
                        }
                    } else if (childE.tagName() == "category") {
                        if (auto t = childE.firstChild().toText(); !t.isNull()) {
                            categories.emplaceBack(t.data());
                        }
                    } else if (childE.tagName() == "return") {
                        // Has attributes for type, varname, etype
                        returnParam = childE.attribute("type", "");
                        returnName = childE.attribute("varname", "");
                        if (childE.hasAttribute("etype")) {
                            QString etype = childE.attribute("etype", "");
                            QString pretty = typesPretty.contains(etype) ? typesPretty[etype] : std::move(etype);
                            if (pretty == "M128" || pretty == "M256" || pretty == "M512") {
                                if (returnParam.contains("void")) {
                                    pretty.clear();
                                } else if (returnParam.contains("__m128i") || returnParam.contains("__m256i") ||
                                    returnParam.contains("__m512i")) {
                                    pretty = "Integer (variable)";
                                } else if (returnParam.contains("__m128") || returnParam.contains("__m256") ||
                                    returnParam.contains("__m512")) {
                                    pretty = typesPretty["FP32"];
                                } else if (returnParam.contains("__m128d") || returnParam.contains("__m256d") ||
                                    returnParam.contains("__m512d")) {
                                    pretty = typesPretty["FP64"];
                                } else if (returnParam.contains("__m128h") || returnParam.contains("__m256h") ||
                                    returnParam.contains("__m512h")) {
                                    pretty = typesPretty["FP16"];
                                } else {
#ifdef _DEBUG
                                    qDebug() << "Unknown type: " << returnParam;
#endif
                                    pretty = "";
                                }
                            }
                            if (!pretty.isEmpty() && !types.contains(pretty)) {
                                types.emplaceBack(std::move(pretty));
                            }
                        }
                    } else if (childE.tagName() == "parameter") {
                        // Can have multiple parameters
                        // Has same attributes as 'return'
                        QString typeParam = childE.attribute("type", "");
                        QString nameParam = childE.attribute("varname", "");
                        parameters.emplaceBack(qMakePair(typeParam, nameParam));
                        if (childE.hasAttribute("etype")) {
                            if (QString etype = childE.attribute("etype", ""); etype != "IMM") {
                                QString pretty = typesPretty.contains(etype) ? typesPretty[etype] : std::move(etype);
                                if (pretty == "M128" || pretty == "M256" || pretty == "M512") {
                                    if (typeParam.contains("void")) {
                                        pretty.clear();
                                    } else if (typeParam.contains("__m128i") || typeParam.contains("__m256i") ||
                                        typeParam.contains("__m512i")) {
                                        pretty = "Integer (variable)";
                                    } else if (typeParam.contains("__m128") || typeParam.contains("__m256") ||
                                        typeParam.contains("__m512")) {
                                        pretty = typesPretty["FP32"];
                                    } else if (typeParam.contains("__m128d") || typeParam.contains("__m256d") ||
                                        typeParam.contains("__m512d")) {
                                        pretty = typesPretty["FP64"];
                                    } else if (typeParam.contains("__m128h") || typeParam.contains("__m256h") ||
                                        typeParam.contains("__m512h")) {
                                        pretty = typesPretty["FP16"];
                                    } else {
#ifdef _DEBUG
                                        qDebug() << "Unknown type: " << typeParam;
#endif
                                        pretty = "";
                                    }
                                }
                                if (!pretty.isEmpty() && !types.contains(pretty)) {
                                    types.emplaceBack(std::move(pretty));
                                }
                            }
                        }
                    } else if (childE.tagName() == "description") {
                        if (auto t = childE.firstChild().toText(); !t.isNull()) {
                            description = t.data();
                        }
                    } else if (childE.tagName() == "operation") {
                        if (auto t = childE.firstChild().toText(); !t.isNull()) {
                            operation = t.data().trimmed();
                        }
                    } else if (childE.tagName() == "instruction") {
                        // Has attributes xed, form, name
                        QString xed = childE.attribute("xed", "");
                        if (!xed.isEmpty() && !xeds.contains(xed)) {
                            xeds.emplaceBack(std::move(xed));
                        }
                        if (!instruction.isEmpty()) {
                            instruction += ',';
                        }
                        instruction += childE.attribute("name", "");
                    } else if (childE.tagName() == "header") {
                        if (auto t = childE.firstChild().toText(); !t.isNull()) {
                            header = t.data();
                        }
                    }
                }
            }
            if (types.isEmpty()) {
                types.append("Other");
            }
            // Skip SVML as its intel compiler only
            if (tech == "SVML" || tech == "KNC") {
                continue;
            }

            // Get corresponding value from uops
            QList<Measurements> measurements;
            if (!xeds.isEmpty()) {
                // uops info stores instructions by extension name (cpuid)
                bool found = false;
                bool secondLoop = false;
                QString cpuid;
                for (auto& j : cpuids) {
                    cpuid = j; // Generally the last one is fine
                }
                // Perform required conversions between intrinsic and uops arch names
                if (cpuid == "ADX") {
                    cpuid = "ADOX_ADCX";
                } else if (cpuid == "AVX2") {
                    // Allow searching through AVX subdomains
                    cpuid = "AVX";
                } else if (cpuid.startsWith("AVX512")) {
                    // Allow searching through AVX512 subdomains
                    cpuid = "AVX512";
                }
                while (true) {
                    for (auto k = root2.firstChild(); !k.isNull() && !found; k = k.nextSibling()) {
                        if (auto elem = k.toElement(); elem.tagName() == "extension") {
                            // Check if the child tag name matches our requested cpuid
                            if (secondLoop || elem.attribute("name").startsWith(cpuid)) {
                                // Search for element containing required xed
                                for (auto child = elem.firstChild(); !child.isNull(); child = child.nextSibling()) {
                                    for (const auto& xed : xeds) {
                                        if (auto childE = child.toElement(); !found && !childE.isNull() &&
                                            childE.tagName() == "instruction" && childE.attribute("iform") == xed) {
                                            found = true;
                                            for (auto child2 = child.firstChild(); !child2.isNull();
                                                 child2 = child2.nextSibling()) {
                                                if (auto child2E = child2.toElement();
                                                    !child2E.isNull() && child2E.tagName() == "architecture") {
                                                    QString arch = child2E.attribute("name");
                                                    QString archPretty = archsPretty.contains(arch) ?
                                                        archsPretty[arch] :
                                                        std::move(arch);
                                                    for (auto child3 = child2.firstChild(); !child3.isNull();
                                                         child3 = child3.nextSibling()) {
                                                        if (auto child3E = child3.toElement();
                                                            !child3E.isNull() && child3E.tagName() == "measurement") {
                                                            uint32_t uops = child3E.attribute("uops").toUInt();
                                                            QString ports = child3E.attribute("ports");

                                                            float throughput = (child3E.hasAttribute("TP")) ?
                                                                child3E.attribute("TP").toFloat() :
                                                                child3E.attribute("TP_unrolled").toFloat();

                                                            // Calculate latency
                                                            uint32_t latency = UINT_MAX;
                                                            uint32_t latencyTrue = UINT_MAX;
                                                            uint32_t latencyMemory =
                                                                UINT_MAX; // Additional latency associated
                                                                          // with using a memory address
                                                            for (auto child4 = child3.firstChild(); !child4.isNull();
                                                                 child4 = child4.nextSibling()) {
                                                                if (auto child4E = child4.toElement();
                                                                    !child4E.isNull() &&
                                                                    child4E.tagName() == "latency") {
                                                                    if (child4E.hasAttribute("cycles")) {
                                                                        if (child4E.attribute("target_op").toUInt() ==
                                                                                1 &&
                                                                            child4E.attribute("start_op").toUInt() ==
                                                                                1 &&
                                                                            child4E.attribute("cycles").toUInt() > 0) {
                                                                            latencyTrue =
                                                                                child4E.attribute("cycles").toUInt();
                                                                        } else {
                                                                            latency = std::max(
                                                                                latency == UINT_MAX ? 0 : latency,
                                                                                child4E.attribute("cycles").toUInt());
                                                                        }
                                                                    } else if (child4E.hasAttribute("cycles_mem")) {
                                                                        latencyMemory = std::max(
                                                                            latencyMemory == UINT_MAX ? 0 :
                                                                                                        latencyMemory,
                                                                            child4E.attribute("cycles_mem").toUInt());
                                                                        latencyMemory = std::max(latencyMemory,
                                                                            child4E.attribute("cycles_addr").toUInt());
                                                                    }
                                                                }
                                                            }

                                                            // Fix latency for cases when different operand ordering
                                                            // resulted in different results
                                                            if (latencyTrue != UINT_MAX) {
                                                                latency = latencyTrue;
                                                            }
                                                            // Fix if only memory latency is provided
                                                            if (latency == UINT_MAX) {
                                                                std::swap(latency, latencyMemory);
                                                            }

                                                            // Add to list
                                                            measurements.emplaceBack(std::move(archPretty), latency,
                                                                latencyMemory, throughput, uops, std::move(ports));
                                                            break;
                                                        }
                                                    }
                                                }
                                            }

                                            // Check if shutdown has been called
                                            if (parentApp->getLoaded()) {
                                                return false;
                                            }
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if (secondLoop) {
                        // Fix for known broken xed lookups
                        bool repeat = false;
                        for (auto& xed : xeds) {
                            if (auto replace = xedConversions.constFind(xed); replace != xedConversions.constEnd()) {
                                xed = replace.value();
                                repeat = true;
                            }
                        }
                        if (repeat) {
                            secondLoop = false;
                            continue;
                        }
                        break;
                    }
                    // Go through the list a second time and this time skip checking the extension
                    secondLoop = true;
                }
#ifdef _DEBUG
                if (!found) {
                    QString xedString;
                    for (const auto& xed : xeds) {
                        if (!xedString.isEmpty()) {
                            xedString += '|';
                        }
                        xedString += xed;
                    }
                    qDebug() << "Intrinsic uops data not found: " + name + ", xed: " + xedString;
                }
#endif
            } else {
#ifdef _DEBUG
                qDebug() << "Intrinsic element in xml file did not have xed element: " + name;
#endif
            }

            // Check if has a valid technology (added in newer version of intrinsic format)
            // TODO: Add tree structure by combining techs with cpuid sub tree
            if (tech.contains("_ALL")) {
#ifdef _DEBUG
                if (cpuids.length() != 1) {
                    qDebug() << "Intrinsic technology value did not map to single cpuid: " + name;
                }
#endif
                tech = cpuids[0];
            }
            if (tech.startsWith("AVX_")) {
                tech = "Other";
            }

            // Add to list of known techs/types
            if (!data.allTechnologies.contains(tech)) {
                data.allTechnologies.append(tech);
            }
            for (auto& j : types) {
                if (!data.allTypes.contains(j)) {
                    data.allTypes.append(j);
                }
            }
            for (auto& j : categories) {
                if (!data.allCategories.contains(j)) {
                    data.allCategories.append(j);
                }
            }

            // Generate pretty display name for intrinsic
            // TODO: Allow for theme specific styling
            QString fullName = "<font color=\"mediumpurple\">";
            fullName += returnParam;
            fullName += "</font>";
            fullName += ' ';
            fullName += name;
            fullName += " (";
            bool first = false;
            for (auto& [parType, parName] : parameters) {
                if (first) {
                    fullName += ", ";
                }
                fullName += "<font color=\"mediumpurple\">";
                fullName += parType;
                fullName += "</font>";
                QString parNameStyled = "<font color=\"lightseagreen\">";
                parNameStyled += parName;
                parNameStyled += "</font>";
                fullName += ' ' + parNameStyled;
                first = true;
                description.replace('"' + parName + '"', parNameStyled);
            }
            fullName += ')';

            // Also update descriptions return variable
            QString parNameStyled = "<font color=\"lightseagreen\">";
            parNameStyled += returnName;
            parNameStyled += "</font>";
            description.replace('"' + returnName + '"', parNameStyled);

            // Create print friendly text
            QString cpuidText, typesText, catText;
            for (auto& j : cpuids) {
                if (!cpuidText.isEmpty()) {
                    cpuidText += ", ";
                }
                cpuidText += j;
            }
            for (auto& j : types) {
                if (!typesText.isEmpty()) {
                    typesText += ", ";
                }
                typesText += j;
            }
            for (auto& j : categories) {
                if (!catText.isEmpty()) {
                    catText += ", ";
                }
                catText += j;
            }

            // Add information to list
            instructions.emplaceBack(std::move(fullName), std::move(name), std::move(description), std::move(operation),
                std::move(header), std::move(cpuidText), std::move(typesText), std::move(catText), std::move(tech),
                std::move(types), std::move(categories), std::move(instruction), std::move(measurements));

            // Check if shutdown has been called
            if (parentApp->getLoaded()) {
                return false;
            }
        }
    }

    addProgress(1.0F);

    // Sort the 'all' lists
    data.allTypes.sort();
    data.allCategories.sort();
    data.allTechnologies.sort();

    // Try and sort technology by age
    QList<QString> sortTechnologies;
    for (auto& j : data.allTechnologies) {
        if (j == "MMX" || j == "AVX") {
            sortTechnologies.emplaceFront(std::move(j));
        } else if (j.startsWith("SSE")) {
            auto find = sortTechnologies.indexOf("AVX");
            sortTechnologies.emplace(find, std::move(j));
        } else if (j == "SSSE3") {
            auto find = sortTechnologies.indexOf("SSE3");
            sortTechnologies.emplace(find + 1, std::move(j));
        } else if (j == "AVX2") {
            auto find = sortTechnologies.indexOf("AVX");
            sortTechnologies.emplace(find + 1, std::move(j));
        } else if (j == "AVX_VNNI") {
            auto find = sortTechnologies.indexOf("AVX2");
            sortTechnologies.emplace(find + 1, std::move(j));
        } else if (j.startsWith("AVX-512")) {
            auto find = sortTechnologies.indexOf("AMX");
            sortTechnologies.emplace(find, std::move(j));
        } else if (!j.isEmpty()) {
            sortTechnologies.emplaceBack(std::move(j));
        }
    }
    data.allTechnologies.swap(sortTechnologies);

    // Generate indexes for stored instructions
    for (auto& i : instructions) {
        const auto tech = static_cast<uint32_t>(data.allTechnologies.indexOf(i.technology));
        QList<uint32_t> types, categories;
        for (auto& j : i.types) {
            types.emplaceBack(static_cast<uint32_t>(data.allTypes.indexOf(j)));
        }
        for (auto& j : i.categories) {
            categories.emplaceBack(static_cast<uint32_t>(data.allCategories.indexOf(j)));
        }
        data.instructions.emplaceBack(std::move(i.fullName), std::move(i.name), std::move(i.description),
            std::move(i.operation), std::move(i.header), std::move(i.cpuidText), std::move(i.typeText),
            std::move(i.categoryText), tech, std::move(types), std::move(categories), std::move(i.instruction),
            std::move(i.measurements));
    }

    std::sort(data.instructions.begin(), data.instructions.end());

#ifndef _DEBUG
    // Delete old cache
    if (QFile file(intrinFile); file.exists()) {
        file.remove();
    }
    if (QFile file(uopsFile); file.exists()) {
        file.remove();
    }
#endif

    return true;
}

bool DataProvider::downloadCache(
    const QString& fileName, const QString& name, QDomDocument& dataXML, const QUrl& url) noexcept
{
    // Check if cached xml file exists
    if (QFile fileCache(fileName); fileCache.exists() && fileCache.open(QIODevice::ReadOnly)) {
        parentApp->setLoadingTitle("Loading " + name + " from cache...");
        addProgress(2.0F);
        QString errorMessage;
        int errorLine = -1, errorColumn = -1;
        if (!dataXML.setContent(&fileCache, &errorMessage, &errorLine, &errorColumn)) {
            qCritical() << "Failed to parse XML: " << errorMessage << " (" << errorLine << ", " << errorColumn << ")";
            // Delete broken file cache
            fileCache.remove();
            parentApp->addOKDialog("Failed to pass " + name + " data", [] {});
            return false;
        }
    } else {
        parentApp->setLoadingTitle("Downloading " + name + "...");
        // Download intrinsic xml
        Downloader dl([this](const float value) { setProgress(value); });
        QByteArray dlData;
        if (!dl.get(url, dlData) || dlData.isEmpty()) {
            parentApp->addOKDialog("Failed to download " + name + " data", [] {});
            return false;
        }

        // Check if shutdown has been called
        if (parentApp->getLoaded()) {
            return false;
        }
        addProgress(1.0F);

        // Convert to xml
        parentApp->setLoadingTitle("Converting " + name + "...");
        QString errorMessage;
        int errorLine = -1, errorColumn = -1;
        if (!dataXML.setContent(dlData, &errorMessage, &errorLine, &errorColumn)) {
            qCritical() << "Failed to parse XML: " << errorMessage << " (" << errorLine << ", " << errorColumn << ")";

            parentApp->addOKDialog("Failed to pass " + name + " data", [] {});
            return false;
        }
        addProgress(1.0F);

        // Check if shutdown has been called
        if (parentApp->getLoaded()) {
            return false;
        }

        // Write out file to disk
        parentApp->setLoadingTitle("Adding " + name + " to cache...");
        if (!fileCache.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qWarning() << "Failed to write XML cache: " + fileName;
        } else {
            QTextStream stream(&fileCache);
            stream << dataXML.toString();
            fileCache.close();
        }

        // Check if shutdown has been called
        if (parentApp->getLoaded()) {
            return false;
        }
    }

    addProgress(1.0F);
    return true;
}
