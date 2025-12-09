#include "backends/recording.h"
#include "dbusinterface.h"
#include <QDBusInterface>
#include <QDBusReply>
#include <iostream>

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
        iface.call("startRecording");
    }
    else {
        iface.call("stopRecording");
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