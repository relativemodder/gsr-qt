#pragma once
#include <QObject>
#include <QDBusInterface>
#include <QDBusConnection>
#include <iostream>
#include "dbusinterface.h"

class DBusBackend : public QObject {
    Q_OBJECT
public:
    explicit DBusBackend(QObject *parent = nullptr);
    virtual ~DBusBackend() = default;

protected:
    QDBusInterface* getDBusInterface();
    void logDBusError(const QString& operation);

private:
    QDBusInterface* m_dbusInterface = nullptr;
};
