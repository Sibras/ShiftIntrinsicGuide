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
#include "MeasurementModel.h"
#include "Version.h"

#include <QQmlContext>

Application::Application(int argc, char* argv[], QObject* parent) noexcept
    : QObject(parent)
    , app(argc, argv)
    , technologiesModel(this)
    , typesModel(this)
    , categoriesModel(this)
    , intrinsicsModel(this)
    , version(SIG_VERSION_STR)
    , provider(this)
{}

Application::~Application() noexcept
{
    loaded = true; // Use loaded to shutdown any running data init
    if (dataLoad.get() != nullptr) {
        dataLoad->waitForFinished();
    }
}

int Application::run() noexcept
{
    // Register the GUI data model
    engine.rootContext()->setContextProperty("application", this);
    engine.rootContext()->setContextProperty("intrinsicsModel", &intrinsicsModel);
    intrinsicProxyModel.setSourceModel(&intrinsicsModel);
    engine.rootContext()->setContextProperty("intrinsicProxyModel", &intrinsicProxyModel);
    engine.rootContext()->setContextProperty("technologiesModel", &technologiesModel);
    engine.rootContext()->setContextProperty("typesModel", &typesModel);
    engine.rootContext()->setContextProperty("categoriesModel", &categoriesModel);
    qmlRegisterUncreatableType<TechnologyModel>("ShiftIntrinsicGuide", 1, 0, "TechnologyModel", "");
    qmlRegisterUncreatableType<CategoryModel>("ShiftIntrinsicGuide", 1, 0, "CategoryModel", "");
    qmlRegisterUncreatableType<TypeModel>("ShiftIntrinsicGuide", 1, 0, "TypeModel", "");
    qmlRegisterUncreatableType<MeasurementModel>("ShiftIntrinsicGuide", 1, 0, "MeasurementModel", "");

    // Load the UI with the default QML file
    engine.load(QUrl(QStringLiteral("qrc:/qml/source/main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return 1;
    }

    // Queue up the model initialisation
    dataLoad = std::make_unique<QFuture<bool>>();
    watcher = std::make_unique<QFutureWatcher<bool>>();
    connect(watcher.get(), &QFutureWatcher<bool>::finished, this, &Application::setupData);
    QTimer::singleShot(1000, [this] {
        *dataLoad = QtConcurrent::run([this] { return provider.setup(); });
        watcher->setFuture(*dataLoad);
    });

    return QGuiApplication::exec();
}

void Application::resetData() noexcept
{
    // Delete old cache
    if (QFile fileCache("./dataCache"); fileCache.exists()) {
        fileCache.remove();
    }

    setLoadingTitle("Reloading...");
    setProgress(0.0F);
    setLoaded(false);

    // Queue up the model initialisation
    dataLoad = std::make_unique<QFuture<bool>>();
    watcher = std::make_unique<QFutureWatcher<bool>>();
    connect(watcher.get(), &QFutureWatcher<bool>::finished, this, &Application::setupData);
    QTimer::singleShot(1000, [this] {
        *dataLoad = QtConcurrent::run([this] { return provider.setup(); });
        watcher->setFuture(*dataLoad);
    });
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
        emit notifyLoadedChanged();
    }
}

QString Application::getLoadingTitle() const noexcept
{
    return loading;
}

void Application::setLoadingTitle(const QString& title) noexcept
{
    loading = title;
    emit notifyLoadingTitleChanged();
}

QString Application::getVersion() const noexcept
{
    return version;
}

QString Application::getDataVersion() const noexcept
{
    return dataVersion;
}

void Application::setupData() noexcept
{
    if (dataLoad->result()) {
        // Setup data models
        technologiesModel.load(provider.getData().allTechnologies);
        typesModel.load(provider.getData().allTypes);
        categoriesModel.load(provider.getData().allCategories);
        intrinsicsModel.load(provider.getData().instructions);

        intrinsicProxyModel.load(technologiesModel.allTechnologies, typesModel.allTypes, categoriesModel.allCategories);

        connect(&technologiesModel, &TechnologyModel::technologyChanged, &intrinsicProxyModel,
            &IntrinsicProxyModel::filterUpdated);
        connect(&typesModel, &TypeModel::typesChanged, &intrinsicProxyModel, &IntrinsicProxyModel::filterUpdated);
        connect(&categoriesModel, &CategoryModel::categoriesyChanged, &intrinsicProxyModel,
            &IntrinsicProxyModel::filterUpdated);

        dataVersion = std::move(provider.getData().version);

        // Clear data provider
        provider.clear();

        // Remove future
        disconnect(watcher.get(), &QFutureWatcher<bool>::finished, this, &Application::setupData);
        dataLoad = nullptr;
        watcher = nullptr;

        // Update UI
        emit notifyDataVersionChanged();
        setProgress(1.0F);
        setLoaded(true);
    } else {
        setLoaded(true);
        addOKDialog("Failed to get intrinsic data", [] {});
    }
}
