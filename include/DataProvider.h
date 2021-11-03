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
#include <QDomDocument>
#include <QList>

class Application;

class DataProvider
{
public:
    DataProvider(const DataProvider& other) = delete;

    DataProvider(DataProvider&& other) noexcept = delete;

    DataProvider& operator=(const DataProvider& other) = delete;

    DataProvider& operator=(DataProvider&& other) noexcept = delete;

    /**
     * Constructor.
     * @param [in] parent (Optional) If non-null, the parent.
     */
    explicit DataProvider(Application* parent) noexcept;

    /**
     * Gets the data.
     */
    bool setup() noexcept;

    /**
     * Sets the progress to the internal value offset by a progress value.
     * @param value The progress value.
     */
    void setProgress(float value) const noexcept;

    /**
     * Increment the internal progress value.
     * @param value The value to increment by.
     */
    void addProgress(float value) noexcept;

private:
    /**
     * Loads this model from stored data.
     */
    [[nodiscard]] bool load() noexcept;

    /**
     * Store the model to stored data.
     */
    [[nodiscard]] bool store() noexcept;

    /**
     * Creates this model by retrieving new data.
     */
    [[nodiscard]] bool create() noexcept;

    /**
     * Downloads to cache or loads from cache if already exists.
     * @param          fileName Filename of the cache file.
     * @param          name     The name of the download/cache.
     * @param [in,out] dataXML  The data XML.
     * @param          url      URL of the resource.
     * @returns True if it succeeds, false if it fails.
     */
    [[nodiscard]] bool downloadCache(
        const QString& fileName, const QString& name, QDomDocument& dataXML, const QUrl& url);

    /** The uops measurements */
    struct Measurements
    {
        QString arch;        /**< The processor architecture the measurement is for */
        uint32_t latency;    /**< The measured latency */
        uint32_t latencyMem; /**< The measured maximum latency of memory operations */
        uint32_t throughput; /**< The measured throughput */
        uint32_t uops;       /**< The instruction uops */
        QString ports;       /**< The instruction ports */

        friend QDataStream& operator<<(QDataStream& out, const struct Measurements& other)
        {
            out << other.arch << other.latency << other.latencyMem << other.throughput << other.uops << other.ports;
            return out;
        }

        friend QDataStream& operator>>(QDataStream& in, struct Measurements& other)
        {
            in >> other.arch >> other.latency >> other.latencyMem >> other.throughput >> other.uops >> other.ports;
            return in;
        }
    };

    /** An intrinsics data. */
    struct Instruction
    {
        QString name;                     /**< The intricics name */
        QString description;              /**< The description */
        QString operation;                /**< The pseudo code operation */
        QString header;                   /**< The instructions required include header */
        QString technology;               /**< The required technology (e.g. AVX etc.) */
        QList<QString> types;             /**< The types of data the intrinsic operates on (e.g. Integer/Float etc.) */
        QList<QString> categories;        /**< The category of operation (e.g. Arithmetic etc.) */
        QString instruction;              /**< The intrinsics assembly equivalent */
        QList<Measurements> measurements; /**< The list of measurements */

        friend QDataStream& operator<<(QDataStream& out, const struct Instruction& other)
        {
            out << other.name << other.description << other.operation << other.header << other.technology << other.types
                << other.categories << other.instruction << other.measurements;
            return out;
        }

        friend QDataStream& operator>>(QDataStream& in, struct Instruction& other)
        {
            in >> other.name >> other.description >> other.operation >> other.header >> other.technology >>
                other.types >> other.categories >> other.instruction >> other.measurements;
            return in;
        }
    };

    struct Data
    {
        QList<Instruction> instructions; /**< The list of all known intrinsics */
        QList<QString> allTypes;         /**< The list of all known intrinsic types */
        QList<QString> allCategories;    /**< The list of all known intrinsic categories */
        QString version;                 /**< The intrinsic list version */
        QDate date;                      /**< The intrinsic list date */

        friend QDataStream& operator<<(QDataStream& out, const struct Data& other)
        {
            out << other.instructions << other.allTypes << other.allCategories << other.version << other.date;
            return out;
        }

        friend QDataStream& operator>>(QDataStream& in, struct Data& other)
        {
            in >> other.instructions >> other.allTypes >> other.allCategories >> other.version >> other.date;
            return in;
        }
    };

    Data data;                     /**< The data */
    float progress = 0.0f;         /**< Stored value indicating total progress of all loading operations */
    float progressModifier = 1.0f; /**< The progress modifier used to scale incoming progress values */
    Application* parentApp;
};
