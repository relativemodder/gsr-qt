#include "backends/recording.h"
#include "dbusinterface.h"
#include <QDBusInterface>
#include <QDBusReply>
#include <iostream>
#include <qdbuspendingcall.h>
#include "backends/shutdownnotifier.h"

RecordingBackend::RecordingBackend(QObject *parent)
    : QObject{parent}
{}

RecordingBackend* RecordingBackend::instance()
{
    static RecordingBackend p;
    return &p;
}

bool RecordingBackend::recordingActive() 
{
    auto connection = DBusInterface::instance()->getConnection();
    QDBusInterface iface(APP_ID, "/", APP_ID, *connection);

    auto active = iface.property("recordingActive");

    if (active.isValid()) {
        return active.toBool();
    }

    std::cerr << "Failed to get recordingActive from daemon: " << qPrintable(connection->lastError().message()) << "\n";
    return false;
}

bool RecordingBackend::recordingPaused() 
{
    auto connection = DBusInterface::instance()->getConnection();
    QDBusInterface iface(APP_ID, "/", APP_ID, *connection);

    auto paused = iface.property("recordingPaused");

    if (paused.isValid()) {
        return paused.toBool();
    }

    std::cerr << "Failed to get recordingPaused from daemon: " << qPrintable(connection->lastError().message()) << "\n";
    return false;
}

void RecordingBackend::toggleRecording(bool state) 
{
    auto connection = DBusInterface::instance()->getConnection();
    QDBusInterface iface(APP_ID, "/", APP_ID, *connection);

    if (state) {
        iface.asyncCall("startRecording");
    }
    else {
        iface.asyncCall("stopRecording");
    }
}

void RecordingBackend::togglePause() 
{
    auto connection = DBusInterface::instance()->getConnection();
    QDBusInterface iface(APP_ID, "/", APP_ID, *connection);

    iface.asyncCall("toggleRecordingPause");
}

void RecordingBackend::subToChangesSignal()
{
    auto connection = DBusInterface::instance()->getConnection();
    connection->connect(
        APP_ID, "/", APP_ID,
        "recordingActiveChanged",
        this,
        SLOT(onRecordingActiveChanged())
    );
    connection->connect(
        APP_ID, "/", APP_ID,
        "recordingPausedChanged",
        this,
        SLOT(onRecordingPausedChanged())
    );
}

void RecordingBackend::onRecordingActiveChanged() {
    emit recordingActiveChanged();
}

void RecordingBackend::onRecordingPausedChanged() {
    emit recordingPausedChanged();
}

QList<QString> RecordingBackend::getCaptureOptions() {
    auto connection = DBusInterface::instance()->getConnection();
    QDBusInterface iface(APP_ID, "/", APP_ID, *connection);

    QDBusReply<QList<QString>> result = iface.call("getCaptureOptions");
    
    if (result.isValid()) {
        return result.value();
    }
    else {
        std::cerr << "Failed to get capture options" << std::endl;
        return {};
    }
}