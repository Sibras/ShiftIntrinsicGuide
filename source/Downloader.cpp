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

#include "Downloader.h"

#include "Application.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtConcurrent>

Downloader::Downloader(std::function<void(float)> setProgress) noexcept
    : QObject(nullptr)
    , callback(std::move(setProgress))
{
    manager = new QNetworkAccessManager(this);
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    connect(manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
}

Downloader::~Downloader() noexcept
{
    delete manager;
}

bool Downloader::get(const QUrl& url, QByteArray& retData) noexcept
{
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "Mozilla Firefox");

    QNetworkReply* reply = manager->get(request);
    connect(reply, &QNetworkReply::downloadProgress, this, &Downloader::downloadProgress);
    timer.start(10000); // 10000ms wait for reply
    loop.exec();

    bool fail = false;
    if (nullptr == reply) {
        // Error. we probably timed out i.e SIGNAL(finished()) did not happen
        // this handles above indicated case (1)
        fail = true;
    } else if (QNetworkReply::NoError != reply->error()) {
        qCritical() << "Failed to download file: " << reply->errorString();
        fail = true;
    } else {
        retData = reply->readAll();
    }

    if (callback != nullptr) {
        callback(1.0f);
    }
    if (reply) {
        delete reply;
    }
    return !fail;
}

void Downloader::downloadProgress(const qint64 bytesReceived, const qint64 bytesTotal)
{
    // Reset timer when progress is detected
    timer.start(10000);

    if (callback != nullptr) {
        const float progress = static_cast<float>(bytesReceived) / static_cast<float>(bytesTotal);
        callback(progress);
    }
}
