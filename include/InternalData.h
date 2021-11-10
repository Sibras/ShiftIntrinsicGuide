#pragma once
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

#include <QDate>
#include <QList>
#include <QString>

/** The uops measurements */
class Measurements
{
public:
    Measurements() noexcept = default;

    Measurements(const Measurements& other) noexcept = default;

    Measurements(Measurements&& other) noexcept = default;

    Measurements& operator=(const Measurements& other) noexcept = default;

    Measurements& operator=(Measurements&& other) noexcept = default;

    Measurements(QString&& newArch, const uint32_t newLatency, const uint32_t newLatencyMem,
        const uint32_t newThroughput, const uint32_t newUops, QString newPorts)
        : arch(std::forward<QString>(newArch))
        , latency(newLatency)
        , latencyMem(newLatencyMem)
        , throughput(newThroughput)
        , uops(newUops)
        , ports(std::forward<QString>(newPorts))
    {}

    QString arch;            /**< The processor architecture the measurement is for */
    uint32_t latency = 0;    /**< The measured latency */
    uint32_t latencyMem = 0; /**< The measured maximum latency of memory operations */
    uint32_t throughput = 0; /**< The measured throughput */
    uint32_t uops = 0;       /**< The instruction uops */
    QString ports;           /**< The instruction ports */

    friend QDataStream& operator<<(QDataStream& out, const Measurements& other)
    {
        out << other.arch << other.latency << other.latencyMem << other.throughput << other.uops << other.ports;
        return out;
    }

    friend QDataStream& operator>>(QDataStream& in, Measurements& other)
    {
        in >> other.arch >> other.latency >> other.latencyMem >> other.throughput >> other.uops >> other.ports;
        return in;
    }
};

Q_DECLARE_METATYPE(Measurements);

/** An intrinsics data. */
class Instruction
{
public:
    Instruction() noexcept = default;

    Instruction(const Instruction& other) noexcept = default;

    Instruction(Instruction&& other) noexcept = default;

    Instruction& operator=(const Instruction& other) noexcept = default;

    Instruction& operator=(Instruction&& other) noexcept = default;

    Instruction(QString&& newFullName, QString&& newName, QString&& newDescription, QString&& newOperation,
        QString&& newHeader, QString&& newTechnology, QList<QString>&& newTypes, QList<QString>&& newCategories,
        QString&& newInstruction, QList<Measurements>&& newMeasurements)
        : fullName(std::forward<QString>(newFullName))
        , name(std::forward<QString>(newName))
        , description(std::forward<QString>(newDescription))
        , operation(std::forward<QString>(newOperation))
        , header(std::forward<QString>(newHeader))
        , technology(std::forward<QString>(newTechnology))
        , types(std::forward<QStringList>(newTypes))
        , categories(std::forward<QStringList>(newCategories))
        , instruction(std::forward<QString>(newInstruction))
        , measurements(std::forward<QList<Measurements>>(newMeasurements))
    {}

    QString fullName;                 /**< Intrinsics name combined with return and parameters */
    QString name;                     /**< The intrinsics name */
    QString description;              /**< The description */
    QString operation;                /**< The pseudo code operation */
    QString header;                   /**< The instructions required include header */
    QString technology;               /**< The required technology (e.g. AVX etc.) */
    QList<QString> types;             /**< The types of data the intrinsic operates on (e.g. Integer/Float etc.) */
    QList<QString> categories;        /**< The category of operation (e.g. Arithmetic etc.) */
    QString instruction;              /**< The intrinsics assembly equivalent */
    QList<Measurements> measurements; /**< The list of measurements */
};

class InstructionIndexed
{
public:
    InstructionIndexed() noexcept = default;

    InstructionIndexed(const InstructionIndexed& other) noexcept = default;

    InstructionIndexed(InstructionIndexed&& other) noexcept = default;

    InstructionIndexed& operator=(const InstructionIndexed& other) noexcept = default;

    InstructionIndexed& operator=(InstructionIndexed&& other) noexcept = default;

    InstructionIndexed(QString&& newFullName, QString&& newName, QString&& newDescription, QString&& newOperation,
        QString&& newHeader, const uint32_t newTechnology, QList<uint32_t>&& newTypes, QList<uint32_t>&& newCategories,
        QString&& newInstruction, QList<Measurements>&& newMeasurements)
        : fullName(std::forward<QString>(newFullName))
        , name(std::forward<QString>(newName))
        , description(std::forward<QString>(newDescription))
        , operation(std::forward<QString>(newOperation))
        , header(std::forward<QString>(newHeader))
        , technology(newTechnology)
        , types(std::forward<QList<uint32_t>>(newTypes))
        , categories(std::forward<QList<uint32_t>>(newCategories))
        , instruction(std::forward<QString>(newInstruction))
        , measurements(std::forward<QList<Measurements>>(newMeasurements))
    {}

    QString fullName;           /**< Intrinsics name combined with return and parameters */
    QString name;               /**< The intrinsics name */
    QString description;        /**< The description */
    QString operation;          /**< The pseudo code operation */
    QString header;             /**< The instructions required include header */
    uint32_t technology = 0;    /**< The required technology (e.g. AVX etc.). Indexes into allTechnologies */
    QList<uint32_t> types;      /**< The data types operated on (e.g. Integer/Float etc.). Indexes into allTypes */
    QList<uint32_t> categories; /**< The category of operation (e.g. Arithmetic etc.). Indexes into allCategories */
    QString instruction;        /**< The intrinsics assembly equivalent */
    QList<Measurements> measurements; /**< The list of measurements */

    friend QDataStream& operator<<(QDataStream& out, const InstructionIndexed& other)
    {
        out << other.fullName << other.name << other.description << other.operation << other.header << other.technology
            << other.types << other.categories << other.instruction << other.measurements;
        return out;
    }

    friend QDataStream& operator>>(QDataStream& in, InstructionIndexed& other)
    {
        in >> other.fullName >> other.name >> other.description >> other.operation >> other.header >>
            other.technology >> other.types >> other.categories >> other.instruction >> other.measurements;
        return in;
    }

    friend bool operator<(const InstructionIndexed& left, const InstructionIndexed& right)
    {
        return left.name < right.name;
    }
};

Q_DECLARE_METATYPE(InstructionIndexed);

class InternalData
{
public:
    InternalData() noexcept = default;

    InternalData(const InternalData& other) noexcept = default;

    InternalData(InternalData&& other) noexcept = default;

    InternalData& operator=(const InternalData& other) noexcept = default;

    InternalData& operator=(InternalData&& other) noexcept = default;

    QList<QString> allTechnologies;         /**< The list of all known intrinsic technologies */
    QList<QString> allTypes;                /**< The list of all known intrinsic types */
    QList<QString> allCategories;           /**< The list of all known intrinsic categories */
    QList<InstructionIndexed> instructions; /**< The list of all known intrinsics */
    QString version;                        /**< The intrinsic list version */
    QDate date;                             /**< The intrinsic list date */

    friend QDataStream& operator<<(QDataStream& out, const InternalData& other)
    {
        out << other.allTechnologies << other.allTypes << other.allCategories << other.instructions << other.version
            << other.date;
        return out;
    }

    friend QDataStream& operator>>(QDataStream& in, InternalData& other)
    {
        in >> other.allTechnologies >> other.allTypes >> other.allCategories >> other.instructions >> other.version >>
            other.date;
        return in;
    }
};

Q_DECLARE_METATYPE(InternalData);

class StringChecked
{
public:
    StringChecked() noexcept = default;

    StringChecked(const StringChecked& other) noexcept = default;

    StringChecked(StringChecked&& other) noexcept = default;

    StringChecked& operator=(const StringChecked& other) noexcept = default;

    StringChecked& operator=(StringChecked&& other) noexcept = default;

    explicit StringChecked(QString&& newName) noexcept
        : name(newName)
    {}

    friend bool operator==(const StringChecked& left, const QString& right)
    {
        return left.name == right;
    }

    QString name;
    bool checked = false;
};

Q_DECLARE_METATYPE(StringChecked);
