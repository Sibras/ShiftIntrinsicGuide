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

#include <QAbstractListModel>

class QNetworkReply;

class DataModel final : public QAbstractListModel
{
    Q_OBJECT;

public:
    DataModel(const DataModel& other) = delete;

    DataModel(DataModel&& other) noexcept = delete;

    DataModel& operator=(const DataModel& other) = delete;

    DataModel& operator=(DataModel&& other) noexcept = delete;

    /**
     * Constructor.
     * @param [in] parent (Optional) If non-null, the parent.
     */
    explicit DataModel(QObject* parent = nullptr) noexcept;

    /** Destructor. */
    ~DataModel() override = default;

    /**
     * Get the number of rows in the model.
     * @note Used Automatically by Qt to get number of motors in the list.
     * @param parameter1 (Optional) The first parameter.
     * @return The number of rows.
     */
    [[nodiscard]] int rowCount(const QModelIndex& parameter1) const noexcept override;

    /**
     * Get data for specific value from a list element.
     * @note Used Automatically by Qt to get the value of a data element.
     * @param index Zero-based index of the list item in the model.
     * @param role  (Optional) The data element to retrieve.
     * @return A QVariant.
     */
    [[nodiscard]] QVariant data(const QModelIndex& index, int role) const noexcept override;

    /**
     * Get the Role names for item in a list element.
     * @note Used Automatically by Qt to get string names of each data value (used for labelling).
     * @return The requested list of names.
     */
    [[nodiscard]] QHash<int, QByteArray> roleNames() const noexcept override;

    /**
     * Get flags for the element at specified index.
     * @note Used Automatically by Qt to get the available operations on each list element.
     * @param index Zero-based index of the list item in the model.
     * @return The flags.
     */
    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex& index) const noexcept override;

private:
};
