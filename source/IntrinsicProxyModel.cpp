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
    // Check if model has been initialised
    if (allTechnologies == nullptr) {
        return false;
    }

    // Check if the index is valid
    const QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    if (!index.isValid()) {
        return false;
    }

    if (!noTechChecked &&
        !(*allTechnologies)[static_cast<qsizetype>(
                                sourceModel()->data(index, IntrinsicModel::IntrinsicRoleTechnology).toUInt())]
             .checked) {
        return false;
    }
    bool valType = noTypeChecked;
    if (!valType) {
        for (const auto& j : sourceModel()->data(index, IntrinsicModel::IntrinsicRoleTypes).toList()) {
            valType = (*allTypes)[static_cast<qsizetype>(j.toUInt())].checked;
            if (valType) {
                break;
            }
        }
    }
    if (!valType) {
        return false;
    }
    bool valCat = noCatsChecked;
    if (!valCat) {
        for (const auto& j : sourceModel()->data(index, IntrinsicModel::IntrinsicRoleCategories).toList()) {
            valCat = (*allCategories)[static_cast<qsizetype>(j.toUInt())].checked;
            if (valCat) {
                break;
            }
        }
    }
    if (!valCat) {
        return false;
    }

    // Check the search string
    if (!search.isEmpty() &&
        !sourceModel()->data(index, IntrinsicModel::IntrinsicRoleName).toString().contains(search)) {
        return false;
    }
    return true;
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
    beginResetModel();
    allTechnologies = &technologies;
    allTypes = &types;
    allCategories = &categories;
    endResetModel();
}

void IntrinsicProxyModel::filterUpdated()
{
    beginResetModel();
    // Check if any options are checked at all and cache the result for faster searching
    noTechChecked = true;
    for (const auto& i : *allTechnologies) {
        if (i.checked) {
            noTechChecked = false;
            break;
        }
    }
    noTypeChecked = true;
    for (const auto& i : *allTypes) {
        if (i.checked) {
            noTypeChecked = false;
            break;
        }
    }
    noCatsChecked = true;
    for (const auto& i : *allCategories) {
        if (i.checked) {
            noCatsChecked = false;
            break;
        }
    }
    endResetModel();
}

void IntrinsicProxyModel::setFilterExpression(const QString& filter)
{
    beginResetModel();
    search = filter;
    endResetModel();
}
