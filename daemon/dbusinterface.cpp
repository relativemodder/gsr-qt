#include "dbusinterface.h"
#include "processes/gsrargs.h"
#include "processes/gsrcli.h"
#include "processes/overlayprocess.h"
#include "windowing/activewindow.h"
#include <iostream>

DBusInterface::DBusInterface(QObject *parent)
    : QObject{parent}
{}

DBusInterface* DBusInterface::instance()
{
    static DBusInterface p;
    return &p;
}

void DBusInterface::toggleShow()
{
    OverlayProcess::instance()->toggleShow();
}

void DBusInterface::setConnection(QDBusConnection* conn)
{
    connection = conn;
}

void DBusInterface::setActiveWindowTitle(QString title) {
    if (title.isEmpty()) {
        return;
    }

    if (title == "GPU Screen Recorder Overlay") {
        return;
    }

    GameWindowInfo info = ActiveWindow::instance()->info();
    info.title = title;

    ActiveWindow::instance()->setActiveWindowInfo(info);

    activeWindowTitleChanged();
}

QString DBusInterface::windowTitle() const {
    QString title = ActiveWindow::instance()->info().title;
    return title;
}

QString DBusInterface::getActiveWindowTitle() {
    GameWindowInfo info = ActiveWindow::instance()->info();
    return info.title;
}

void DBusInterface::setActiveWindowFullscreen(bool isFullscreen) {
    GameWindowInfo info = ActiveWindow::instance()->info();
    info.isFullscreen = isFullscreen;

    ActiveWindow::instance()->setActiveWindowInfo(info);
}

void DBusInterface::setupRecordingListening() {
    auto cli = GSRCli::instance();

    connect(cli, &GSRCli::recordingChanged, this, [this] {
        emit this->recordingActiveChanged();
    });
    
    connect(cli, &GSRCli::recordingPausedChanged, this, [this] {
        emit this->recordingPausedChanged();
    });
    
}

bool DBusInterface::recordingActive() const {
    return GSRCli::instance()->isRecording();
}

void DBusInterface::startRecording() {
    GSRCli::instance()->startRecording();
}

void DBusInterface::stopRecording() {
    GSRCli::instance()->stopRecording();
}

void DBusInterface::toggleRecordingPause()
{
    GSRCli::instance()->toggleRecordingPause();
}

bool DBusInterface::recordingPaused() const
{
    return GSRCli::instance()->isRecordingPaused();
}

QList<QString> DBusInterface::getCaptureOptions() {
    if (!cachedCaptureOptions.empty()) {
        return cachedCaptureOptions;
    }
    cachedCaptureOptions = {};

    for (auto option : GSRCli::instance()->getCaptureOptions()) 
    {
        QString opt = option.name;
        if (option.resolution.has_value()) {
            opt.append("|" + option.resolution.value());
        }
        cachedCaptureOptions.append(opt);
    }

    return cachedCaptureOptions;
}