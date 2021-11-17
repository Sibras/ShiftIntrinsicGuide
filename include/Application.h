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

#include "CategoryModel.h"
#include "DataProvider.h"
#include "IntrinsicModel.h"
#include "IntrinsicProxyModel.h"
#include "TechnologyModel.h"
#include "TypeModel.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtConcurrent>

class Application final : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool hasOKDialog READ getHasOKDialog NOTIFY hasOKDialogChanged)

    Q_PROPERTY(QString OKDialogTitle READ getOKDialogTitle NOTIFY notifyOKDialogTitleChanged)

    Q_PROPERTY(float progress READ getProgress WRITE setProgress NOTIFY notifyProgressChanged)

    Q_PROPERTY(bool isLoaded READ getLoaded NOTIFY notifyLoadedChanged)

    Q_PROPERTY(QString loadingTitle READ getLoadingTitle NOTIFY notifyLoadingTitleChanged)

    Q_PROPERTY(QString version READ getVersion NOTIFY notifyVersionChanged)

    Q_PROPERTY(QString dataVersion READ getDataVersion NOTIFY notifyDataVersionChanged)

public:
    Application(const Application&) = delete;

    Application(Application&&) = delete;

    Application& operator=(const Application&) = delete;

    Application& operator=(Application&&) = delete;

    /** Default constructor. */
    explicit Application(QObject* parent = nullptr) noexcept;

    /** Destructor. */
    ~Application() noexcept override;

    /**
     * Runs the GUI.
     * @returns An int error code.
     */
    int run() noexcept;

    /**
     * Loads/Re-loads intrinsic data.
     */
    Q_INVOKABLE void resetData() noexcept;

    /**
     * Displays an dialog with an OK button.
     * @param title    The title for the dialog box.
     * @param callback The callback function called when the dialog is closed.
     */
    void addOKDialog(const QString& title, const std::function<void()>& callback);

    /**
     * Gets whether a new OK dialog is required.
     * @return True if there is, false otherwise.
     */
    bool getHasOKDialog() const noexcept;

    /** Notify the GUI that the available OK dialog's has changed. */
    Q_SIGNAL void hasOKDialogChanged() const;

    /**
     * Gets the title for a requested OK dialog.
     * @return The dialog title.
     */
    QString getOKDialogTitle() const noexcept;

    /** Notify the GUI that the available OK dialog title has changed. */
    Q_SIGNAL void notifyOKDialogTitleChanged() const;

    /** Callback, called when a OK dialog is accepted. */
    Q_SLOT void acceptOKCallback() noexcept;

    /**
     * Gets the progress value.
     * @returns The progress.
     */
    float getProgress() const noexcept;

    /**
     * Sets the progress value.
     * @param newProgress The progress.
     */
    void setProgress(float newProgress) noexcept;

    /** Notify the GUI that the progress has changed. */
    Q_SIGNAL void notifyProgressChanged() const;

    /**
     * Gets whether the data is loaded.
     * @return True if it is, false if it is not.
     */
    bool getLoaded() const noexcept;

    /**
     * Sets whether data is in loaded state.
     * @param newLoaded (Optional) True if the engine is loaded.
     */
    void setLoaded(bool newLoaded = true) noexcept;

    /** Notify the GUI that the loaded has changed. */
    Q_SIGNAL void notifyLoadedChanged() const;

    /**
     * Gets the title for the loading text.
     * @return The loading title.
     */
    QString getLoadingTitle() const noexcept;

    /**
     * Sets the loading title.
     * @param title The new title.
     */
    void setLoadingTitle(const QString& title) noexcept;

    /** Notify the GUI that the available loading title has changed. */
    Q_SIGNAL void notifyLoadingTitleChanged() const;

    /**
     * Gets the current program version.
     * @return The version string.
     */
    QString getVersion() const noexcept;

    /** Notify the GUI that the version has changed. */
    Q_SIGNAL void notifyVersionChanged() const;

    /**
     * Gets the current intrinsic data version.
     * @return The version string.
     */
    QString getDataVersion() const noexcept;

    /** Notify the GUI that the version has changed. */
    Q_SIGNAL void notifyDataVersionChanged() const;

private:
    /**
     * Sets up the internal data models
     */
    Q_SLOT void setupData() noexcept;

    QGuiApplication app;
    QQmlApplicationEngine engine;
    TechnologyModel technologiesModel;
    TypeModel typesModel;
    CategoryModel categoriesModel;
    IntrinsicModel intrinsicsModel;
    IntrinsicProxyModel intrinsicProxyModel;
    QString version;
    QString dataVersion;

    struct OKDialogData final
    {
        QString title;
        std::function<void()> callback;
    };

    QQueue<OKDialogData> queueOK;
    QMutex mutexOK;
    std::atomic<float> progress = 0.0F;
    std::atomic_bool loaded = false;
    QString loading = "Loading...";
    DataProvider provider;
    std::unique_ptr<QFuture<bool>> dataLoad = nullptr;
    std::unique_ptr<QFutureWatcher<bool>> watcher = nullptr;
};
