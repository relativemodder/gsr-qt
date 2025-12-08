#include "backends/activewindow.h"
#include <QDBusInterface>
#include <QDBusReply>
#include <iostream>
#include "dbusinterface.h"

ActiveWindow::ActiveWindow(QObject *parent)
    : QObject{parent}
{}

ActiveWindow* ActiveWindow::instance()
{
    static ActiveWindow p;
    return &p;
}

QString ActiveWindow::getTitle() const {
    return m_title;
}

void ActiveWindow::setTitle(const QString& t) {
    if (m_title != t) {
        m_title = t;
        emit titleChanged();
    }
}

void ActiveWindow::requestWindowTitle() 
{
    auto connection = DBusInterface::instance()->getConnection();
    QDBusInterface iface(APP_ID, "/", APP_ID, *connection);

    QDBusReply<QString> title = iface.call("getActiveWindowTitle");
    if (title.isValid()) {
        ActiveWindow::instance()->setTitle(title.value());
    } else {
        std::cerr << "Failed to get active window title from daemon: " << qPrintable(connection->lastError().message()) << "\n";
    }
}

void ActiveWindow::onActiveWindowTitleChanged() 
{
    requestWindowTitle();
}

void ActiveWindow::subToChangesSignal() 
{
    auto connection = DBusInterface::instance()->getConnection();
    connection->connect(
        APP_ID,
        "/",
        APP_ID,
        "activeWindowTitleChanged",
        this,
        SLOT(onActiveWindowTitleChanged())
    );
}