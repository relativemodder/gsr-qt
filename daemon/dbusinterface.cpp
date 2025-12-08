#include <iostream>
#include <qdbusinterface.h>
#include <qdbusreply.h>
#include "dbusinterface.h"
#include "processes/overlayprocess.h"
#include "windowing/activewindow.h"

DBusInterface::DBusInterface(QObject *parent)
    : QObject{parent}
{}

DBusInterface* DBusInterface::instance()
{
    static DBusInterface p;
    return &p;
}

void DBusInterface::toggleShow()
{
    OverlayProcess::instance()->toggleShow();
}

void DBusInterface::setConnection(QDBusConnection* conn)
{
    connection = conn;
}

void DBusInterface::setActiveWindowTitle(QString title) {
    if (title.isEmpty()) {
        std::cerr << "Received empty title, ignoring.\n";
        return;
    }

    if (title == "GPU Screen Recorder Overlay") {
        std::cerr << "Received overlay title, ignoring.\n";
        return;
    }

    GameWindowInfo info = ActiveWindow::instance()->info();
    info.title = title;

    ActiveWindow::instance()->setActiveWindowInfo(info);

    std::cout << "Set active window title to: " << title.toStdString() << std::endl;
}

QString DBusInterface::getActiveWindowTitle() {
    GameWindowInfo info = ActiveWindow::instance()->info();
    return info.title;
}