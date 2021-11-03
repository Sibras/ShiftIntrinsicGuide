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

#include "Application.h"

#include "DataProvider.h"

#include <QQmlContext>

int argc = 0;

Application::Application(QObject* parent) noexcept
    : QObject(parent)
    , app(argc, nullptr)
    , data(this)
{}

Application::~Application() noexcept
{
    loaded = true; // Use loaded to shutdown any running data init
    dataLoad.waitForFinished();
}

int Application::run() noexcept
{
    // Register the GUI data model
    engine.rootContext()->setContextProperty("application", this);
    engine.rootContext()->setContextProperty("dataModel", &data);

    // Load the UI with the default QML file
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (!engine.rootObjects().isEmpty()) {
        // return 1;
    }

    // Queue up the model initialisation
    QTimer::singleShot(1000, [&] { dataLoad = QtConcurrent::run([this] { setupData(); }); });

    return app.exec();
}

void Application::addOKDialog(const QString& title, const std::function<void()>& callback)
{
    // Add the new dialog to the queue
    QMutexLocker lock(&mutexOK);
    queueOK.enqueue({title, callback});
    // Check if already a dialog being displayed
    if (queueOK.size() == 1) {
        emit notifyOKDialogTitleChanged();
        emit hasOKDialogChanged();
    }
}

bool Application::getHasOKDialog() const noexcept
{
    return !queueOK.empty();
}

QString Application::getOKDialogTitle() const noexcept
{
    if (!queueOK.empty()) {
        return queueOK.front().title;
    }
    return "";
}

void Application::acceptOKCallback() noexcept
{
    OKDialogData infoData;
    {
        QMutexLocker lock(&mutexOK);
        // Call the callback function
        infoData = queueOK.takeFirst();
        // Check if there are more dialogs to be displayed
        if (!queueOK.empty()) {
            // If there are any pending OK dialogs they need to be re-enabled
            emit notifyOKDialogTitleChanged();
            emit hasOKDialogChanged();
        }
    }
    if (infoData.callback != nullptr) {
        infoData.callback();
    }
}

float Application::getProgress() const noexcept
{
    return progress.load();
}

void Application::setProgress(const float newProgress) noexcept
{
    progress = fmin(fabs(newProgress), 1.0F);
    emit notifyProgressChanged();
}

bool Application::getLoaded() const noexcept
{
    return loaded.load();
}

void Application::setLoaded(const bool newLoaded) noexcept
{
    if (bool expected = !newLoaded; loaded.compare_exchange_strong(expected, newLoaded)) {
        emit isLoadedChanged();
    }
}

QString Application::getLoadingTitle() const noexcept
{
    return loading;
}

void Application::setLoadingTitle(const QString& title)
{
    loading = title;
    emit notifyLoadingTitleChanged();
}

void Application::setupData()
{
    if (DataProvider provider(this); provider.setup()) {
        // TODO: setup data models

        // Update UI
        setProgress(1.0f);
        setLoaded(true);
    } else {
        addOKDialog("Failed to get intrinsic data", [] {});
    }
}
