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

#include "CategoryModel.h"

#include "Application.h"

CategoryModel::CategoryModel(QObject* parent) noexcept
    : QAbstractListModel(parent)
{}

int CategoryModel::rowCount(const QModelIndex& /*parameter1*/) const noexcept
{
    return static_cast<int>(allCategories.count());
}

QVariant CategoryModel::data(const QModelIndex& index, const int role) const noexcept
{
    if (index.row() >= 0 && index.row() < allCategories.count()) {
        switch (role) {
            case CategoryRoleName:
                return allCategories.at(index.row()).name;
            case CategoryRoleChecked:
                return allCategories.at(index.row()).checked;
            default:
                break;
        }
    }
    return QVariant();
}

QHash<int, QByteArray> CategoryModel::roleNames() const noexcept
{
    static const QHash<int, QByteArray> roles{
        {CategoryRoleName, "categoryName"}, {CategoryRoleChecked, "categoryChecked"}};
    return roles;
}

Qt::ItemFlags CategoryModel::flags(const QModelIndex& index) const noexcept
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

bool CategoryModel::setData(const QModelIndex& index, const QVariant& value, const int role) noexcept
{
    if (index.row() < allCategories.count()) {
        if (role == CategoryRoleChecked) {
            // Update the value
            allCategories[index.row()].checked = value.toBool();
            emit dataChanged(index, index, {role});
            emit categoriesyChanged();
            return true;
        }
        // This is not allowed
        QByteArray data(roleNames()[role]);
        qDebug() << "UI Tried to update menu item: " << data.data();
    }
    return false;
}

void CategoryModel::load(QList<QString>& categories) noexcept
{
    emit beginInsertRows(QModelIndex(), 0, static_cast<int>(categories.count()) - 1);
    // Copy in data
    for (auto& i : categories) {
        allCategories.emplaceBack(std::move(i));
    }
    emit endInsertRows();
}
