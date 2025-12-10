#include "backends/recording.h"
#include <QDBusReply>
#include <qdbuspendingcall.h>

RecordingBackend::RecordingBackend(QObject *parent)
    : DBusBackend{parent}
{}

RecordingBackend* RecordingBackend::instance()
{
    static RecordingBackend p;
    return &p;
}

bool RecordingBackend::recordingActive()
{
    auto active = getDBusInterface()->property("recordingActive");

    if (active.isValid()) {
        return active.toBool();
    }

    logDBusError("recordingActive property");
    return false;
}

bool RecordingBackend::recordingPaused()
{
    auto paused = getDBusInterface()->property("recordingPaused");

    if (paused.isValid()) {
        return paused.toBool();
    }

    logDBusError("recordingPaused property");
    return false;
}

void RecordingBackend::toggleRecording(bool state)
{
    if (state) {
        getDBusInterface()->asyncCall("startRecording");
    } else {
        getDBusInterface()->asyncCall("stopRecording");
    }
}

void RecordingBackend::togglePause()
{
    getDBusInterface()->asyncCall("toggleRecordingPause");
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
    QDBusReply<QList<QString>> result = getDBusInterface()->call("getCaptureOptions");

    if (result.isValid()) {
        return result.value();
    } else {
        logDBusError("getCaptureOptions");
        return {};
    }
}
