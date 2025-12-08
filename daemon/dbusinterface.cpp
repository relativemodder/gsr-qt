#include "dbusinterface.h"
#include "processes/overlayprocess.h"
#include "windowing/activewindow.h"
#include <iostream>

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

    activeWindowTitleChanged();

    std::cout << "Set active window title to: " << title.toStdString() << std::endl;
}

QString DBusInterface::windowTitle() const {
    QString title = ActiveWindow::instance()->info().title;
    return title;
}

QString DBusInterface::getActiveWindowTitle() {
    GameWindowInfo info = ActiveWindow::instance()->info();
    return info.title;
}

void DBusInterface::setActiveWindowFullscreen(bool isFullscreen) {
    GameWindowInfo info = ActiveWindow::instance()->info();
    info.isFullscreen = isFullscreen;

    ActiveWindow::instance()->setActiveWindowInfo(info);

    std::cout << "Set active window fullscreen to: " << (isFullscreen ? "true" : "false") << std::endl;
}