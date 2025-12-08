#include "dbusinterface.h"
#include "shutdownnotifier.h"

DBusInterface::DBusInterface(QObject *parent)
    : QObject{parent}
{}

DBusInterface* DBusInterface::instance()
{
    static DBusInterface p;
    return &p;
}

void DBusInterface::setConnection(QDBusConnection* conn)
{
    connection = conn;
}

void DBusInterface::gtfo() {
    ShutdownNotifier::instance()->intendedClose();
}