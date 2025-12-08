#pragma once

#include <QDBusContext>
#include <QDBusMessage>
#include <QDBusConnection>
#include <QObject>
#include <qdbusconnection.h>

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

public slots:
    void toggleShow();
    void setActiveWindowTitle(QString title);
    QString getActiveWindowTitle();
};