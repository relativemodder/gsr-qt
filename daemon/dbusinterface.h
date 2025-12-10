#pragma once

#include <QDBusContext>
#include <QDBusMessage>
#include <QDBusConnection>
#include <QObject>
#include <QList>
#include "processes/gsrcli.h"

class DBusInterface : public QObject, protected QDBusContext
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", APP_ID)

private:
    QDBusConnection* connection;

public:
    explicit DBusInterface(QObject *parent = nullptr);
    static DBusInterface* instance();

    void setConnection(QDBusConnection* conn);
    QDBusConnection* getConnection() const { return connection; }

    void setupRecordingListening();

    Q_PROPERTY(QString windowTitle READ getActiveWindowTitle NOTIFY activeWindowTitleChanged);
    QString windowTitle() const;

    Q_PROPERTY(bool recordingActive READ recordingActive NOTIFY recordingActiveChanged);
    bool recordingActive() const;

public slots:
    void toggleShow();
    void setActiveWindowTitle(QString title);
    QString getActiveWindowTitle();

    void setActiveWindowFullscreen(bool isFullscreen);

    void startRecording();
    void stopRecording();

    QList<QString> getCaptureOptions();

signals:
    void activeWindowTitleChanged();
    void recordingActiveChanged();
};