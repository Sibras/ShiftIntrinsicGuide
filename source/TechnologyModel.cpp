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

#include "TechnologyModel.h"

#include "Application.h"

TechnologyModel::TechnologyModel(QObject* parent) noexcept
    : QAbstractListModel(parent)
{}

int TechnologyModel::rowCount(const QModelIndex& /*parameter1*/) const noexcept
{
    return static_cast<int>(allTechnologies.count());
}

QVariant TechnologyModel::data(const QModelIndex& index, const int role) const noexcept
{
    if (index.row() >= 0 && index.row() < allTechnologies.count()) {
        switch (role) {
            case TechnologyRoleName:
                return allTechnologies.at(index.row()).name;
            case TechnologyRoleChecked:
                return allTechnologies.at(index.row()).checked;
            default:
                break;
        }
    }
    return QVariant();
}

QHash<int, QByteArray> TechnologyModel::roleNames() const noexcept
{
    static const QHash<int, QByteArray> roles{
        {TechnologyRoleName, "technologyName"}, {TechnologyRoleChecked, "technologyChecked"}};
    return roles;
}

Qt::ItemFlags TechnologyModel::flags(const QModelIndex& index) const noexcept
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

bool TechnologyModel::setData(const QModelIndex& index, const QVariant& value, const int role) noexcept
{
    if (index.row() < allTechnologies.count()) {
        if (role == TechnologyRoleChecked) {
            // Update the value
            allTechnologies[index.row()].checked = value.toBool();
            emit dataChanged(index, index, {role});
            emit technologyChanged();
            return true;
        }
        // This is not allowed
        QByteArray data(roleNames()[role]);
        qDebug() << "UI Tried to update menu item: " << data.data();
    }
    return false;
}

void TechnologyModel::load(QList<QString>& technologies) noexcept
{
    emit beginInsertRows(QModelIndex(), 0, static_cast<int>(technologies.count()) - 1);
    // Copy in data
    for (auto& i : technologies) {
        allTechnologies.emplaceBack(std::move(i));
    }
    emit endInsertRows();
}
