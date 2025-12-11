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
#include <qtenvironmentvariables.h>
#include <unistd.h>
#include <QDBusConnection>
#include <QDBusError>
#include <QDBusReply>
#include "dbusinterface.h"
#include "x11hacks.h"
#include <gsrsettings.h>
#include "backends/activewindow.h"
#include "backends/recording.h"
#include <LayerShellQt/window.h>
#include <QCommandLineParser>

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

    QCommandLineParser parser;
    parser.setApplicationDescription("GSR Qt Overlay");
    parser.addHelpOption();

    QCommandLineOption keyboardInteractivityOpt("kbint", "Keyboard interactivity", "on-demand|exclusive|none", "on-demand");
    QCommandLineOption layerOpt("layer", "Compositor's layer", "top|overlay", "top");

    parser.addOption(keyboardInteractivityOpt);
    parser.addOption(layerOpt);

    parser.process(app);

    QString keyboardInteractivity = parser.value(keyboardInteractivityOpt);
    QString overlayLayer = parser.value(layerOpt);

    auto connection = QDBusConnection::sessionBus();

    if (!connection.isConnected()) {
        std::cerr << "Cannot connect to the D-Bus session bus.\n"
                 "To start it, run:\n"
                 "\teval `dbus-launch --auto-syntax`\n";
        return 1;
    }

    if (!connection.registerService(OVERLAY_APP_ID)) {
        auto qwerr = connection.lastError();
        auto qwr = qwerr.message();
        std::cerr << "Error registering service: " << qwerr.type() << std::endl;
        
        connection.unregisterService(OVERLAY_APP_ID);
        connection.registerService(OVERLAY_APP_ID);
    }
    
    DBusInterface::instance()->setConnection(&connection);

    connection.registerObject("/", DBusInterface::instance(), QDBusConnection::ExportAllSlots);

    install_signal_handler();

    QQmlApplicationEngine engine;

    ActiveWindow::instance()->requestWindowTitle();
    ActiveWindow::instance()->subToChangesSignal();
    RecordingBackend::instance()->subToChangesSignal();

    engine.rootContext()->setContextProperty(QStringLiteral("shutdownNotifier"), ShutdownNotifier::instance());
    engine.rootContext()->setContextProperty(QStringLiteral("activeWindow"), ActiveWindow::instance());
    engine.rootContext()->setContextProperty(QStringLiteral("recording"), RecordingBackend::instance());
    engine.rootContext()->setContextProperty(QStringLiteral("settings"), &GSRSettings::instance());
    engine.rootContext()->setContextProperty(QStringLiteral("keyboardInteractivityString"), keyboardInteractivity);
    engine.rootContext()->setContextProperty(QStringLiteral("overlayLayerString"), overlayLayer);

    std::cout << "Context properties are set\n";

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

    return app.exec();
}
