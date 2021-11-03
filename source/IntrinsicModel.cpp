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

IntrinsicModel::IntrinsicModel(QObject* parent) noexcept
    : QAbstractListModel(parent)
{}

int IntrinsicModel::rowCount(const QModelIndex& parameter1) const noexcept
{
    return 0;
}

QVariant IntrinsicModel::data(const QModelIndex& index, int role) const noexcept
{
    return QVariant();
}

QHash<int, QByteArray> IntrinsicModel::roleNames() const noexcept
{
    static const QHash<int, QByteArray> roles{};
    return roles;
}

Qt::ItemFlags IntrinsicModel::flags(const QModelIndex& /*index*/) const noexcept
{
    return Qt::NoItemFlags;
}

void IntrinsicModel::load(QList<Instruction>& data) noexcept
{}
