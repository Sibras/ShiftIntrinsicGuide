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

#include "IntrinsicProxyModel.h"

#include "Application.h"

IntrinsicProxyModel::IntrinsicProxyModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{}

bool IntrinsicProxyModel::filterAcceptsRow(const int sourceRow, const QModelIndex& sourceParent) const
{
    if (allTechnologies == nullptr) {
        return false;
    }

    const QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    if (!index.isValid()) {
        return false;
    }

    const auto valTech = std::find(allTechnologies->cbegin(), allTechnologies->cend(),
        sourceModel()->data(index, IntrinsicModel::IntrinsicRoleTechnology).toString());
    bool valType = false;
    for (const auto& j : sourceModel()->data(index, IntrinsicModel::IntrinsicRoleTypes).toStringList()) {
        for (const auto& i : *allTypes) {
            if (i.name == j) {
                valType = i.checked;
                break;
            }
        }
        if (valType) {
            break;
        }
    }
    bool valCat = false;
    for (const auto& j : sourceModel()->data(index, IntrinsicModel::IntrinsicRoleCategories).toStringList()) {
        for (const auto& i : *allCategories) {
            if (i.name == j) {
                valCat = i.checked;
                break;
            }
        }
        if (valCat) {
            break;
        }
    }
    return valTech->checked && valType && valCat;
}

bool IntrinsicProxyModel::lessThan(const QModelIndex& left, const QModelIndex& right) const
{
    const QVariant leftData = sourceModel()->data(left);
    const QVariant rightData = sourceModel()->data(right);

    if (leftData.userType() == QMetaType::QString) {
        return leftData.toString() < rightData.toString();
    }
    return false;
}

void IntrinsicProxyModel::load(const QList<StringChecked>& technologies, const QList<StringChecked>& types,
    const QList<StringChecked>& categories) noexcept
{
    // Copy in data
    allTechnologies = &technologies;
    allTypes = &types;
    allCategories = &categories;
    invalidateFilter();
}

void IntrinsicProxyModel::filterUpdated()
{
    invalidateFilter();
}
