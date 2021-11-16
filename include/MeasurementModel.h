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

#include <QAbstractTableModel>

class MeasurementModel final : public QAbstractTableModel
{
    Q_OBJECT

public:
    MeasurementModel(const MeasurementModel& other) = delete;

    MeasurementModel(MeasurementModel&& other) noexcept = delete;

    MeasurementModel& operator=(const MeasurementModel& other) = delete;

    MeasurementModel& operator=(MeasurementModel&& other) noexcept = delete;

    /**
     * Constructor.
     * @param [in] parent (Optional) If non-null, the parent.
     */
    explicit MeasurementModel(QObject* parent = nullptr) noexcept;

    /**
     * Constructor.
     * @param [in,out] data   The data to load.
     * @param [in]     parent (Optional) If non-null, the parent.
     */
    explicit MeasurementModel(QList<Measurements>&& data, QObject* parent = nullptr) noexcept;

    /** Destructor. */
    ~MeasurementModel() override = default;

    /**
     * Get the number of rows in the model.
     * @note Used Automatically by Qt to get number of items in the list.
     * @param parameter1 The first parameter.
     * @return The number of rows.
     */
    [[nodiscard]] int rowCount(const QModelIndex& parameter1) const noexcept override;

    /**
     * Get the number of columns in the model.
     * @note Used Automatically by Qt to get number of values for each item.
     * @param parameter1 The first parameter.
     * @return The number of columns.
     */
    [[nodiscard]] int columnCount(const QModelIndex& parameter1) const noexcept override;

    /**
     * Get the header information for the table.
     * @note Used Automatically by Qt to get column names for the table.
     * @param section     This is ignored.
     * @param orientation Whether displayed horizontal or vertical.
     * @param role        The data element to retrieve.
     * @return The number of columns.
     */
    [[nodiscard]] Q_INVOKABLE QVariant headerData(
        int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const noexcept override;

    /**
     * Get data for specific value from a list element.
     * @note Used Automatically by Qt to get the value of a data element.
     * @param index Zero-based index of the list item in the model.
     * @param role  The data element to retrieve.
     * @return A QVariant.
     */
    [[nodiscard]] QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const noexcept override;

private:
    QList<Measurements> measurements; /**< The list of all measurements */
};
