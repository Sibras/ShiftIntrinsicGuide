﻿/**
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

#define QT_NO_DEBUG_OUTPUT

#include "DataProvider.h"

#include "Application.h"
#include "Downloader.h"

constexpr uint32_t fileVersion = 0x010000;
constexpr uint32_t fileChecksum = 0xA654BE39;

DataProvider::DataProvider(Application* parent) noexcept
    : parentApp(parent)
{}

void DataProvider::setProgress(const float value) const noexcept
{
    const auto progress2 = progress + value * progressModifier;
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
    bool fail = false;
    // Load any existing cache from disk
    if (!load()) {
        // If no cache found then create new one
        fail = !create();
        if (!fail) {
            // Backup created cache to disk
            (void)store();
        }
    }

    if (!fail) {
        // Remove loading progress from parent
        parentApp->setProgress(1.0f);
    }
    return true;
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
    // Reset UI values to default
    progressModifier = 1.0f;
    progress = 0.0f;
    parentApp->setLoadingTitle("Loading...");
    setProgress(0.0f); // Trigger parent update

    if (QFile fileCache("./dataCache"); fileCache.exists() && fileCache.open(QIODevice::ReadOnly)) {
        // Load data from cache
        parentApp->setLoadingTitle("Loading data from cache...");
        QDataStream in(&fileCache);
        uint32_t check;
        in >> check;
        if (check != fileChecksum) {
            qWarning("File load had invalid checksum");
            return false;
        }
        in >> check;
        if (check != fileVersion) {
            qInfo("File version to old, creating new one");
            return false;
        }
        in.setVersion(QDataStream::Qt_6_2);
        in >> data;
        addProgress(1.0f);
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
        out << fileChecksum;
        out << fileVersion;
        out.setVersion(QDataStream::Qt_6_2);
        out << data;
        addProgress(1.0f);
        return true;
    }
    return false;
}

bool DataProvider::create() noexcept
{
    // Reset UI values to default
    progressModifier = 1.0f / 8.0f;
    progress = 0.0f;
    parentApp->setLoadingTitle("Creating...");
    setProgress(0.0f); // Trigger parent update

    QDomDocument dataXMLIntel, dataXMLUOps;
    if (!downloadCache("./intrin.xml", "Intel Intrinsic Guide", dataXMLIntel,
            QUrl("https://www.intel.com/content/dam/develop/public/us/en/include/intrinsics-guide/data-latest.xml"))) {
        return false;
    }
    if (!downloadCache("./uops.xml", "uops.info", dataXMLUOps, QUrl("https://www.uops.info/instructions.xml"))) {
        return false;
    }

    parentApp->setLoadingTitle("Creating data store...");
    // Scan through intrinsic data for each function
    QDomElement root = dataXMLIntel.documentElement();
    QDomElement root2 = dataXMLUOps.documentElement();
    data.version = root.attribute("version", "3.6.0");
    data.date = QDate::fromString(root.attribute("date", "06/30/2021"), "MM/dd/YYYY");

    QMap<QString, QString> typesPretty = {{"BF16", "BFloat16"}, {"FP16", "Float16 (half)"}, {"FP32", "Float32 (float)"},
        {"FP64", "Float64 (double)"}, {"MASK", "Mask"}, {"SI16", "Integer Signed 16 (int16)"},
        {"SI32", "Integer Signed 32 (int32)"}, {"SI64", "Integer Signed 64 (int64)"},
        {"SI8", "Integer Signed 8 (int8)"}, {"UI16", "Integer Unsigned 16 (uint16)"},
        {"UI32", "Integer Unsigned 32 (uint32)"}, {"UI64", "Integer Unsigned 64 (uint64)"},
        {"UI8", "Integer Unsigned 8 (uint8)"}};

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
            QList<QString> types, cpuids, categories;
            QString description, operation, header, instruction, xed;
            QString returnParam;
            QList<QString> parameters;
            if (name.isEmpty()) {
                qDebug() << "Intrinsic element in xml file did not have name attribute: " + node.toText().data();
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
                        if (childE.hasAttribute("etype")) {
                            QString etype = childE.attribute("etype", "");
                            QString pretty = typesPretty.contains(etype) ? typesPretty[etype] : std::move(etype);
                            if (pretty == "M128" || pretty == "M256" || pretty == "M512") {
                                if (returnParam.contains("void")) {
                                    pretty = "";
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
                                    qDebug() << "Unknown type: " << returnParam;
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
                        parameters.emplaceBack(typeParam + " " + nameParam);
                        if (childE.hasAttribute("etype")) {
                            if (QString etype = childE.attribute("etype", ""); etype != "IMM") {
                                QString pretty = typesPretty.contains(etype) ? typesPretty[etype] : std::move(etype);
                                if (pretty == "M128" || pretty == "M256" || pretty == "M512") {
                                    if (typeParam.contains("void")) {
                                        pretty = "";
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
                                        qDebug() << "Unknown type: " << typeParam;
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
                            operation = t.data();
                        }
                    } else if (childE.tagName() == "instruction") {
                        // Has attributes xed, form, name
                        if (!xed.isEmpty()) {
                            // Can have multiple xeds if intrinsic can map to different instructions
                            qDebug() << "Intrinsic has multiple xeds: " + name + ", xed: " + xed;
                        } else {
                            // Always take the first instruction form
                            xed = childE.attribute("xed", "");
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
            if (!xed.isEmpty()) {
                // uops info stores instructions by extension name (cpuid)
                bool found = false;
                bool secondLoop = false;
                QString cpuid;
                for (auto& j : cpuids) {
                    cpuid = j; // Generally the last one is fine
                }
                // Perform required conversions between intrin and uops arch names
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
                                    if (auto childE = child.toElement(); !childE.isNull() &&
                                        childE.tagName() == "instruction" && childE.attribute("iform") == xed) {
                                        found = true;
                                        bool found2 = false;
                                        for (auto child2 = child.firstChild(); !child2.isNull() && !found2;
                                             child2 = child2.nextSibling()) {
                                            if (auto child2E = child2.toElement();
                                                !child2E.isNull() && child2E.tagName() == "architecture") {
                                                QString arch = child2E.attribute("name");

                                                for (auto child3 = child2.firstChild(); !child3.isNull();
                                                     child3 = child3.nextSibling()) {
                                                    if (auto child3E = child3.toElement();
                                                        !child3E.isNull() && child3E.tagName() == "measurement") {
                                                        uint32_t uops = child3E.attribute("uops").toUInt();
                                                        QString ports = child3E.attribute("ports");

                                                        uint32_t throughput = (child3E.hasAttribute("TP")) ?
                                                            child3E.attribute("TP").toUInt() :
                                                            child3E.attribute("TP_unrolled").toUInt();

                                                        // Calculate latency
                                                        uint32_t latency = 0;
                                                        uint32_t latencyMemory = 0; // Additional latency associated
                                                                                    // with using a memory address
                                                        for (auto child4 = child3.firstChild(); !child4.isNull();
                                                             child4 = child4.nextSibling()) {
                                                            if (auto child4E = child4.toElement();
                                                                !child4E.isNull() && child4E.tagName() == "latency") {
                                                                for (auto attr = 0;
                                                                     attr < child4E.attributes().length(); ++attr) {
                                                                    if (auto attrE =
                                                                            child4E.attributes().item(attr).toElement();
                                                                        !attrE.isNull() &&
                                                                        attrE.tagName().startsWith("cycles")) {
                                                                        if (attrE.tagName() == "cycles") {
                                                                            latency = std::max(
                                                                                latency, attrE.tagName().toUInt());
                                                                        } else {
                                                                            latencyMemory = std::max(latencyMemory,
                                                                                attrE.tagName().toUInt());
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }

                                                        // Add to list
                                                        measurements.emplaceBack(std::move(arch), latency,
                                                            latencyMemory, throughput, uops, std::move(ports));
                                                        found2 = true;
                                                        break;
                                                    }
                                                }
                                            }
                                        }

                                        break;
                                    }
                                }
                            }
                        }
                    }
                    if (secondLoop) {
                        break;
                    }
                    // Go through the list a second time and this time skip checking the extension
                    secondLoop = true;
                }
                if (!found) {
                    qDebug() << "Intrinsic uops data not found: " + name + ", xed: " + xed;
                }
            } else {
                qDebug() << "Intrinsic element in xml file did not have xed element: " + name;
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

            // Add information to list
            QString fullName = returnParam;
            fullName += ' ';
            fullName += name;
            fullName += " (";
            bool first = false;
            for (auto& k : parameters) {
                if (first) {
                    fullName += ", ";
                }
                fullName += k;
                first = true;
            }
            fullName += ')';
            data.instructions.emplaceBack(std::move(fullName), std::move(name), std::move(description),
                std::move(operation), std::move(header), std::move(tech), std::move(types), std::move(categories),
                std::move(instruction), std::move(measurements));
        }
    }

    addProgress(1.0f);

    // Sort the 'all' lists
    data.allTypes.sort();
    data.allCategories.sort();
    data.allTechnologies.sort();

    // Try and sort technology by age
    QList<QString> sortTechnologies;
    for (auto& j : data.allTechnologies) {
        if (j == "MMX") {
            sortTechnologies.emplaceFront(std::move(j));
        } else if (j.startsWith("SSE")) {
            auto find = sortTechnologies.indexOf("AVX");
            sortTechnologies.emplace(find, std::move(j));
        } else if (j == "SSSE3") {
            auto find = sortTechnologies.indexOf("SSE3");
            sortTechnologies.emplace(find + 1, std::move(j));
        } else if (j == "AVX") {
            sortTechnologies.emplaceFront(std::move(j));
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

    // TODO:****************
    // Delete any cache files from disk

    return true;
}

bool DataProvider::downloadCache(
    const QString& fileName, const QString& name, QDomDocument& dataXML, const QUrl& url) noexcept
{
    // Check if cached xml file exists
    if (QFile fileCache(fileName); fileCache.exists() && fileCache.open(QIODevice::ReadOnly)) {
        parentApp->setLoadingTitle("Loading " + name + " from cache...");
        addProgress(2.0f);
        QString errorMessage;
        int errorLine, errorColumn;
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
        Downloader dl([this](const float value) { this->setProgress(value); });
        QByteArray dlData;
        if (!dl.get(url, dlData) || dlData.isEmpty()) {
            parentApp->addOKDialog("Failed to download " + name + " data", [] {});
            return false;
        }

        // Check if shutdown has been called
        if (parentApp->getLoaded()) {
            return false;
        }
        addProgress(1.0f);

        // Convert to xml
        parentApp->setLoadingTitle("Converting " + name + "...");
        QString errorMessage;
        int errorLine, errorColumn;
        if (!dataXML.setContent(dlData, &errorMessage, &errorLine, &errorColumn)) {
            qCritical() << "Failed to parse XML: " << errorMessage << " (" << errorLine << ", " << errorColumn << ")";

            parentApp->addOKDialog("Failed to pass " + name + " data", [] {});
            return false;
        }
        addProgress(1.0f);

        // Check if shutdown has been called
        if (parentApp->getLoaded()) {
            return false;
        }

        // Write out file to disk
        parentApp->setLoadingTitle("Adding " + name + " to cache...");
        if (!fileCache.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Failed to write XML cache: " + fileName;
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

    addProgress(1.0f);
    return true;
}