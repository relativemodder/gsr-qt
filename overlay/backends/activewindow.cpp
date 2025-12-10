#include "backends/activewindow.h"
#include <QDBusReply>

ActiveWindow::ActiveWindow(QObject *parent)
    : DBusBackend{parent}
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
    QDBusReply<QString> title = getDBusInterface()->call("getActiveWindowTitle");
    if (title.isValid()) {
        setTitle(title.value());
    } else {
        logDBusError("getActiveWindowTitle");
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
        APP_ID, "/", APP_ID,
        "activeWindowTitleChanged",
        this,
        SLOT(onActiveWindowTitleChanged())
    );
}
