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

#include "TypeModel.h"

TypeModel::TypeModel(QObject* parent) noexcept
    : QAbstractListModel(parent)
{}

int TypeModel::rowCount(const QModelIndex& /*parameter1*/) const noexcept
{
    return static_cast<int>(allTypes.count());
}

QVariant TypeModel::data(const QModelIndex& index, const int role) const noexcept
{
    if (index.row() < allTypes.count()) {
        switch (role) {
            case TypeRoleName:
                return allTypes.at(index.row()).name;
            case TypeRoleChecked:
                return allTypes.at(index.row()).checked;
            default:
                break;
        }
    }
    return QVariant();
}

QHash<int, QByteArray> TypeModel::roleNames() const noexcept
{
    static const QHash<int, QByteArray> roles{{TypeRoleName, "typeName"}, {TypeRoleChecked, "typeChecked"}};
    return roles;
}

Qt::ItemFlags TypeModel::flags(const QModelIndex& /*index*/) const noexcept
{
    return Qt::NoItemFlags;
}

void TypeModel::load(QList<QString>& types) noexcept
{
    emit beginInsertRows(QModelIndex(), 0, static_cast<int>(types.count()) - 1);
    // Copy in data
    for (auto& i : types) {
        allTypes.emplaceBack(std::move(i));
    }
    emit endInsertRows();
}
