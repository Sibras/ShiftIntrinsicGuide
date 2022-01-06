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

#include "MeasurementModel.h"

#include "Application.h"

MeasurementModel::MeasurementModel(QObject* parent) noexcept
    : QAbstractTableModel(parent)
{}

MeasurementModel::MeasurementModel(QList<Measurements>&& data, QObject* parent) noexcept
    : QAbstractTableModel(parent)
    , measurements(std::forward<QList<Measurements>>(data))
{}

int MeasurementModel::rowCount(const QModelIndex& /*parameter1*/) const noexcept
{
    return static_cast<int>(measurements.count());
}

int MeasurementModel::columnCount(const QModelIndex& /*parameter1*/) const noexcept
{
    return 4;
}

QVariant MeasurementModel::headerData(
    const int section, const Qt::Orientation orientation, const int role) const noexcept
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
                case 0:
                    return QString("Latency");
                case 1:
                    return QString("Throughput");
                case 2:
                    return QString("UOps");
                case 3:
                    return QString("Ports");
                default:
                    break;
            }
        } else {
            if (section < measurements.count()) {
                return measurements.at(section).arch;
            }
        }
    }
    return "";
}

QVariant MeasurementModel::data(const QModelIndex& index, const int role) const noexcept
{
    if (!index.isValid()) {
        return "";
    }
    if (role == Qt::DisplayRole && index.row() >= 0 && index.row() < measurements.count() && index.column() >= 0 &&
        index.column() < 4) {
        switch (index.column()) {
            case 0: {
                if (const auto item = measurements.at(index.row());
                    item.latency == item.latencyMem || item.latencyMem == UINT_MAX) {
                    if (item.latency != UINT_MAX) {
                        return QString::number(measurements.at(index.row()).latency);
                    }
                    return QString();
                } else {
                    QString ret('[');
                    ret += QString::number(measurements.at(index.row()).latency);
                    ret += ';';
                    ret += QString::number(measurements.at(index.row()).latencyMem);
                    ret += ']';
                    return ret;
                }
            }
            case 1:
                return QString::number(measurements.at(index.row()).throughput);
            case 2:
                return QString::number(measurements.at(index.row()).uops);
            case 3:
                return measurements.at(index.row()).ports;
            default:
                break;
        }
    }
    return "";
}
