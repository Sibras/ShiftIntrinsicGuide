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

#include "InternalData.h"

#include <QDomDocument>

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

    /** Clears this object to its blank/initial state. */
    void clear();

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

    /**
     * Gets the internal data.
     * @returns The data.
     */
    [[nodiscard]] InternalData& getData() noexcept;

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

    InternalData data;             /**< The data */
    float progress = 0.0f;         /**< Stored value indicating total progress of all loading operations */
    float progressModifier = 1.0f; /**< The progress modifier used to scale incoming progress values */
    Application* parentApp;
};
