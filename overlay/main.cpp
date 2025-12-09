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
    QCoreApplication::setOrganizationName("Relative");
    QCoreApplication::setApplicationName("gsr-qt");

    QGuiApplication app(argc, argv);
    std::cout << "Created QGuiApplication\n";

    auto connection = QDBusConnection::sessionBus();
    qDebug() << "DBus connection: " << connection.isConnected();

    if (!connection.isConnected()) {
        std::cerr << "Cannot connect to the D-Bus session bus.\n"
                 "To start it, run:\n"
                 "\teval `dbus-launch --auto-syntax`\n";
        return 1;
    }

    qDebug() << "Checked connection";



    if (!connection.registerService(OVERLAY_APP_ID)) {
        auto qwerr = connection.lastError();
        auto qwr = qwerr.message();
        std::cerr << "Error registering service: " << qwerr.type() << std::endl;
        
        connection.unregisterService(OVERLAY_APP_ID);
        connection.registerService(OVERLAY_APP_ID);
    }

    qDebug() << "Registered service";
    
    DBusInterface::instance()->setConnection(&connection);
    qDebug() << "Connection set in DBusInterface";

    connection.registerObject("/", DBusInterface::instance(), QDBusConnection::ExportAllSlots);
    std::cout << "Connected to D-Bus\n";

    install_signal_handler();

    std::cout << "Signal handler is on\n";

    QQmlApplicationEngine engine;

    std::cout << "QQmlApplicationEngine created\n";

    ActiveWindow::instance()->requestWindowTitle();
    ActiveWindow::instance()->subToChangesSignal();

    std::cout << "ActiveWindow requested and subbed\n";

    engine.rootContext()->setContextProperty(QStringLiteral("shutdownNotifier"), ShutdownNotifier::instance());
    engine.rootContext()->setContextProperty(QStringLiteral("activeWindow"), ActiveWindow::instance());

    std::cout << "Context properties are set\n";

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [](QObject *obj, const QUrl &objUrl) {

        std::cout << "Creating a window\n";
            
        QQuickWindow *window = qobject_cast<QQuickWindow*>(obj);

        if (QGuiApplication::platformName() == "xcb") {
            apply_x11_hacks(window);
            window->showFullScreen();
        }
        else {
            window->show();
        }

        
    }, Qt::QueuedConnection);

    std::cout << "Loading MainOverlay\n";

    engine.loadFromModule("GsrQt", "MainOverlay");

    return app.exec();
}
