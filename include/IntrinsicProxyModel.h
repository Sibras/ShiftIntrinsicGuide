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

#include <QSortFilterProxyModel>

class IntrinsicProxyModel final : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    /**
     * Constructor.
     * @param [in] parent (Optional) If non-null, the parent.
     */
    explicit IntrinsicProxyModel(QObject* parent = nullptr);

    /**
     * Filter rows in a list.
     * @note Used Automatically by Qt to filter down in the attached model.
     * @param  sourceRow    Source row.
     * @param  sourceParent Source parent.
     * @returns True if it succeeds, false if it fails.
     */
    [[nodiscard]] bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

    /**
     * Less than comparison.
     * @note Used Automatically by Qt to sort items in the attached model.
     * @param  left  The left.
     * @param  right The right.
     * @returns True if it succeeds, false if it fails.
     */
    [[nodiscard]] bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;

    /**
     * Loads this model.
     * @param [in,out] technologies The data to load.
     * @param [in,out] types The data to load.
     * @param [in,out] categories The data to load.
     * @note This is used to force the model to update all data. This must be run from the primary thread.
     */
    void load(const QList<StringChecked>& technologies, const QList<StringChecked>& types,
        const QList<StringChecked>& categories) noexcept;

    /** Signal that the filter need to be updated */
    Q_SLOT void filterUpdated();

    /**
     * Sets filter expression from search box.
     * @param filter Specifies the filter.
     */
    Q_SLOT void setFilterExpression(const QString& filter);

private:
    const QList<StringChecked>* allTechnologies = nullptr; /**< The list of all known intrinsic technologies */
    const QList<StringChecked>* allTypes = nullptr;        /**< The list of all known intrinsic types */
    const QList<StringChecked>* allCategories = nullptr;   /**< The list of all known intrinsic categories */
    bool noTechChecked = true;                             /**< True if no technologies are checked */
    bool noTypeChecked = true;                             /**< True if no types are checked */
    bool noCatsChecked = true;                             /**< True if no categories are checked */
    QString search;                                        /**< The search string */
};
