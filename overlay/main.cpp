#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQuickWindow>
#include <QQmlContext>
#include <csignal>
#include <iostream>
#include <QWindow>
#include "backends/shutdownnotifier.h"
#include <qcoreapplication.h>
#include <qdbusinterface.h>
#include <unistd.h>
#include <QDBusConnection>
#include <QDBusError>
#include <QDBusReply>
#include "dbusinterface.h"
#include "x11hacks.h"
#include "backends/activewindow.h"

void termination_signal_handler(int signum) {
    if (globalShutdownTimer) {
        QMetaObject::invokeMethod(globalShutdownTimer, "start", Qt::QueuedConnection);
    }
    
    emit ShutdownNotifier::instance()->startShutdownAnimation();
}

void install_signal_handler() {
    struct sigaction sa;
    sa.sa_handler = termination_signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        std::cerr << "Cannot install SIGINT handler!\n";
    }
    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        std::cerr << "Cannot install SIGTERM handler!\n";
    }
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    auto connection = QDBusConnection::sessionBus();

    if (!connection.isConnected()) {
        std::cerr << "Cannot connect to the D-Bus session bus.\n"
                 "To start it, run:\n"
                 "\teval `dbus-launch --auto-syntax`\n";
        return 1;
    }

    if (!connection.registerService(OVERLAY_APP_ID)) {
        qWarning("%s\n", qPrintable(connection.lastError().message()));
        exit(1);
    }
    
    DBusInterface::instance()->setConnection(&connection);
    connection.registerObject("/", DBusInterface::instance(), QDBusConnection::ExportAllSlots);

    install_signal_handler();

    QQmlApplicationEngine engine;

    ActiveWindow::instance()->requestWindowTitle();
    ActiveWindow::instance()->subToChangesSignal();

    engine.rootContext()->setContextProperty(QStringLiteral("shutdownNotifier"), ShutdownNotifier::instance());
    engine.rootContext()->setContextProperty(QStringLiteral("activeWindow"), ActiveWindow::instance());

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [](QObject *obj, const QUrl &objUrl) {
            
        QQuickWindow *window = qobject_cast<QQuickWindow*>(obj);

        if (QGuiApplication::platformName() == "xcb") {
            apply_x11_hacks(window);
            window->showFullScreen();
        }
        else {
            window->show();
        }

        
    }, Qt::QueuedConnection);

    engine.loadFromModule("GsrQt", "MainOverlay");

    std::cout << "ok window created\n";

    return app.exec();
}