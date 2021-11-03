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

#include <QEventLoop>
#include <QMutex>
#include <QNetworkAccessManager>
#include <QObject>
#include <QTimer>
#include <QWaitCondition>

class Application;
class QNetworkReply;

class Downloader final : public QObject
{
    Q_OBJECT;

public:
    /**
     * Constructor
     * @param setProgress (Optional) If non-null, the callback to signal download progress.
     */
    explicit Downloader(std::function<void(float)> setProgress = nullptr) noexcept;

    /** Destructor */
    ~Downloader() noexcept override;

    /**
     * Gets the url data.
     * @param       url     URL of the resource.
     * @param [out] retData Return used to pass back data from request.
     * @returns True if it succeeds, false if it fails.
     */
    bool get(const QUrl& url, QByteArray& retData) noexcept;

private:
    QNetworkAccessManager* manager = nullptr;
    QEventLoop loop;
    QTimer timer;
    std::function<void(float)> callback;

    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
};
