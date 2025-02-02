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

    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/html; charset=UTF-8");
    request.setHeader(QNetworkRequest::UserAgentHeader,
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:65.0) Gecko/20100101 Firefox/65.0");
    request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,/;q=0.8");

    uint32_t retries = 3;
    bool fail = true;
    while (retries != 0) {
        QNetworkReply* reply = manager->get(request);
        connect(reply, &QNetworkReply::downloadProgress, this, &Downloader::downloadProgress);
        timer.start(10000); // 10000ms wait for reply
        loop.exec();

        if (nullptr == reply) {
            // Error. we probably timed out i.e SIGNAL(finished()) did not happen
            // this handles above indicated case (1)
            qCritical() << "Failed to download file: internal error";
        } else if (QNetworkReply::NoError != reply->error()) {
            qCritical() << "Failed to download file: " << reply->errorString();
            delete reply;
        } else {
            retData = reply->readAll();
            delete reply;
            fail = false;
            break;
        }
        --retries;
        if (retries > 0) {
            qCritical() << "Attempting to download file again";
        }
    }

    if (callback != nullptr) {
        callback(1.0f);
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
