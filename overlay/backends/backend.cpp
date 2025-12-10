#include "backends/backend.h"

DBusBackend::DBusBackend(QObject *parent)
    : QObject{parent}
{}

QDBusInterface* DBusBackend::getDBusInterface() {
    if (!m_dbusInterface) {
        auto connection = DBusInterface::instance()->getConnection();
        m_dbusInterface = new QDBusInterface(APP_ID, "/", APP_ID, *connection, this);
    }
    return m_dbusInterface;
}

void DBusBackend::logDBusError(const QString& operation) {
    auto connection = DBusInterface::instance()->getConnection();
    std::cerr << "DBus error in " << operation.toStdString() << ": " << qPrintable(connection->lastError().message()) << "\n";
}
