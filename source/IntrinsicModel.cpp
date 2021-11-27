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

#include "IntrinsicModel.h"

#include "Application.h"
#include "MeasurementModel.h"

IntrinsicModel::IntrinsicModel(QObject* parent) noexcept
    : QAbstractListModel(parent)
{}

int IntrinsicModel::rowCount(const QModelIndex& /*parameter1*/) const noexcept
{
    return static_cast<int>(instructions.count());
}

QVariant IntrinsicModel::data(const QModelIndex& index, const int role) const noexcept
{
    if (index.row() >= 0 && index.row() < instructions.count()) {
        switch (role) {
            case IntrinsicRoleFullName:
                return instructions.at(index.row()).fullName;
            case IntrinsicRoleName:
                return instructions.at(index.row()).name;
            case IntrinsicRoleDescription:
                return instructions.at(index.row()).description;
            case IntrinsicRoleOperation:
                return instructions.at(index.row()).operation;
            case IntrinsicRoleHeader:
                return instructions.at(index.row()).header;
            case IntrinsicRoleCPUIDText:
                return instructions.at(index.row()).cpuidText;
            case IntrinsicRoleTypesText:
                return instructions.at(index.row()).typeText;
            case IntrinsicRoleCategoriesText:
                return instructions.at(index.row()).categoryText;
            case IntrinsicRoleTechnology:
                return instructions.at(index.row()).technology;
            case IntrinsicRoleTypes:
                return QVariant::fromValue(instructions.at(index.row()).types);
            case IntrinsicRoleCategories:
                return QVariant::fromValue(instructions.at(index.row()).categories);
            case IntrinsicRoleInstruction:
                return instructions.at(index.row()).instruction;
            case IntrinsicRoleMeasurements:
                return QVariant::fromValue(static_cast<QObject*>(instructions.at(index.row()).measurements.get()));
            case IntrinsicRoleExpanded:
                return instructions.at(index.row()).expanded;
            default:
                break;
        }
    }
    return {};
}

QHash<int, QByteArray> IntrinsicModel::roleNames() const noexcept
{
    static const QHash<int, QByteArray> roles{{IntrinsicRoleFullName, "intrinsicFullName"},
        {IntrinsicRoleName, "intrinsicName"}, {IntrinsicRoleDescription, "intrinsicDescription"},
        {IntrinsicRoleOperation, "intrinsicOperation"}, {IntrinsicRoleHeader, "intrinsicHeader"},
        {IntrinsicRoleCPUIDText, "intrinsicCPUIDText"}, {IntrinsicRoleTypesText, "intrinsicTypesText"},
        {IntrinsicRoleCategoriesText, "intrinsicCategoriesText"}, {IntrinsicRoleTechnology, "intrinsicTechnology"},
        {IntrinsicRoleTypes, "intrinsicTypes"}, {IntrinsicRoleCategories, "intrinsicCategories"},
        {IntrinsicRoleInstruction, "intrinsicInstruction"}, {IntrinsicRoleMeasurements, "intrinsicMeasurements"},
        {IntrinsicRoleExpanded, "intrinsicExpanded"}};
    return roles;
}

Qt::ItemFlags IntrinsicModel::flags(const QModelIndex& /*index*/) const noexcept
{
    return Qt::NoItemFlags;
}

bool IntrinsicModel::setData(const QModelIndex& index, const QVariant& value, const int role) noexcept
{
    if (index.row() < instructions.count()) {
        if (role == IntrinsicRoleExpanded) {
            // Update the value
            instructions[index.row()].expanded = value.toBool();
            emit dataChanged(index, index, {role});
            return true;
        }
        // This is not allowed
        QByteArray data(roleNames()[role]);
        qDebug() << "UI Tried to update intrinsic item: " << data.data();
    }
    return false;
}

void IntrinsicModel::load(QList<InstructionIndexed>& data) noexcept
{
    if (!instructions.isEmpty()) {
        emit beginResetModel();
        instructions.clear();
        emit endResetModel();
    }
    emit beginInsertRows(QModelIndex(), 0, static_cast<int>(data.count()) - 1);
    // Move in data
    for (auto& i : data) {
        instructions.emplaceBack(std::move(i), reinterpret_cast<QObject*>(this));
    }
    emit endInsertRows();
}

InstructionModeled::InstructionModeled(InstructionIndexed&& base, QObject* parent)
    : fullName(std::forward<QString>(base.fullName))
    , name(std::forward<QString>(base.name))
    , description(std::forward<QString>(base.description))
    , operation(std::forward<QString>(base.operation))
    , header(std::forward<QString>(base.header))
    , cpuidText(std::forward<QString>(base.cpuidText))
    , typeText(std::forward<QString>(base.typeText))
    , categoryText(std::forward<QString>(base.categoryText))
    , technology(base.technology)
    , types(std::forward<QList<uint32_t>>(base.types))
    , categories(std::forward<QList<uint32_t>>(base.categories))
    , instruction(std::forward<QString>(base.instruction))
    , measurements(std::make_shared<MeasurementModel>(std::forward<QList<Measurements>>(base.measurements), parent))
{}
