#include "activewindow.h"
#include "../dbusinterface.h"
#include <QDBusInterface>

ActiveWindow::ActiveWindow(QObject *parent)
    : QObject{parent}
{}

ActiveWindow* ActiveWindow::instance()
{
    static ActiveWindow p;
    return &p;
}

void ActiveWindow::setActiveWindowInfo(const GameWindowInfo& info)
{
    activeWindowInfo = info;
}

void ActiveWindow::applyKWinHacks()
{
    auto connection = DBusInterface::instance()->getConnection();
    QDBusInterface iface("org.kde.KWin", "/Scripting", "org.kde.kwin.Scripting", *connection);

    iface.call("unloadScript", "gsrkwinhelper");

    iface.call("loadScript", "/usr/share/gsr-qt/gsrkwinhelper.js", "gsrkwinhelper");
    iface.call("start");
    
}