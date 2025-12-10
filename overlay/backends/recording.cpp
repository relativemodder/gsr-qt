#include "backends/recording.h"
#include "dbusinterface.h"
#include <QDBusInterface>
#include <QDBusReply>
#include <iostream>
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

    std::cerr << "Failed to get active window title from daemon: " << qPrintable(connection->lastError().message()) << "\n";
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

void RecordingBackend::subToChangesSignal()
{
    auto connection = DBusInterface::instance()->getConnection();
    connection->connect(
        APP_ID, "/", APP_ID,
        "recordingActiveChanged",
        this,
        SLOT(onRecordingActiveChanged())
    );
}

void RecordingBackend::onRecordingActiveChanged() {
    emit recordingActiveChanged();
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