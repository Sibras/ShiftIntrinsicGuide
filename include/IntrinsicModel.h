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

#include <QAbstractListModel>

class MeasurementModel;

class InstructionModeled
{
public:
    friend class IntrinsicModel;

    InstructionModeled(const InstructionModeled& other) noexcept = default;

    InstructionModeled(InstructionModeled&& other) noexcept = default;

    InstructionModeled& operator=(const InstructionModeled& other) noexcept = default;

    InstructionModeled& operator=(InstructionModeled&& other) noexcept = default;

    InstructionModeled(InstructionIndexed&& base, QObject* parent);

    ~InstructionModeled() noexcept = default;

private:
    QString fullName;           /**< Intrinsics name combined with return and parameters */
    QString name;               /**< The intrinsics name */
    QString description;        /**< The description */
    QString operation;          /**< The pseudo code operation */
    QString header;             /**< The instructions required include header */
    QString cpuidText;          /**< The required CPUID`s as user readable text */
    QString typeText;           /**< The required types as user readable text */
    QString categoryText;       /**< The categories of operation as user readable text */
    uint32_t technology = 0;    /**< The required technology (e.g. AVX etc.). Indexes into allTechnologies */
    QList<uint32_t> types;      /**< The data types operated on (e.g. Integer/Float etc.). Indexes into allTypes */
    QList<uint32_t> categories; /**< The category of operation (e.g. Arithmetic etc.). Indexes into allCategories */
    QString instruction;        /**< The intrinsics assembly equivalent */
    std::shared_ptr<MeasurementModel> measurements; /**< The list of measurements */
};

Q_DECLARE_METATYPE(InstructionModeled);

class IntrinsicModel final : public QAbstractListModel
{
    Q_OBJECT

public:
    IntrinsicModel(const IntrinsicModel& other) = delete;

    IntrinsicModel(IntrinsicModel&& other) noexcept = delete;

    IntrinsicModel& operator=(const IntrinsicModel& other) = delete;

    IntrinsicModel& operator=(IntrinsicModel&& other) noexcept = delete;

    enum IntrinsicRole
    {
        IntrinsicRoleFullName = Qt::DisplayRole,
        IntrinsicRoleName = Qt::UserRole,
        IntrinsicRoleDescription,
        IntrinsicRoleOperation,
        IntrinsicRoleHeader,
        IntrinsicRoleCPUIDText,
        IntrinsicRoleTypesText,
        IntrinsicRoleCategoriesText,
        IntrinsicRoleTechnology,
        IntrinsicRoleTypes,
        IntrinsicRoleCategories,
        IntrinsicRoleInstruction,
        IntrinsicRoleMeasurements
    };

    Q_ENUM(IntrinsicRole)

    /**
     * Constructor.
     * @param [in] parent (Optional) If non-null, the parent.
     */
    explicit IntrinsicModel(QObject* parent = nullptr) noexcept;

    /** Destructor. */
    ~IntrinsicModel() override = default;

    /**
     * Get the number of rows in the model.
     * @note Used Automatically by Qt to get number of items in the list.
     * @param parameter1 The first parameter.
     * @return The number of rows.
     */
    [[nodiscard]] int rowCount(const QModelIndex& parameter1) const noexcept override;

    /**
     * Get data for specific value from a list element.
     * @note Used Automatically by Qt to get the value of a data element.
     * @param index Zero-based index of the list item in the model.
     * @param role  The data element to retrieve.
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

    /**
     * Loads this model.
     * @param [in,out] data The data to load.
     */
    void load(QList<InstructionIndexed>& data) noexcept;

private:
    QList<InstructionModeled> instructions; /**< The list of all known intrinsics */
};
