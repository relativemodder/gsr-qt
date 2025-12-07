#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQuickWindow>
#include <QQmlContext>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <csignal>
#include <iostream>
#include <LayerShellQt/shell.h>
#include <LayerShellQt/window.h>
#include <QWindow>
#include "shutdownnotifier.h"
#include <qcoreapplication.h>
#include <unistd.h>

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

void apply_x11_hacks(QQuickWindow *window) {
    if (window) {
        window->setFlags(Qt::FramelessWindowHint | 
                         Qt::WindowStaysOnTopHint | 
                         Qt::BypassWindowManagerHint |
                        Qt::WindowDoesNotAcceptFocus);
        
        window->setColor(QColor(0, 0, 0, 0));
        window->setProperty("_q_showWithoutActivating", true);

        QScreen *screen = QGuiApplication::primaryScreen();
        window->setGeometry(screen->geometry());
        window->show();

        auto *x11Application = qGuiApp->nativeInterface<QNativeInterface::QX11Application>();

        QPlatformNativeInterface *native = QGuiApplication::platformNativeInterface();
        Display *dpy = x11Application->display();
        WId win = window->winId();

        Atom stateAtom = XInternAtom(dpy, "_NET_WM_STATE", False);
        Atom stateAbove = XInternAtom(dpy, "_NET_WM_STATE_ABOVE", False);
        Atom stateFullscreen = XInternAtom(dpy, "_NET_WM_STATE_FULLSCREEN", False);
        Atom stateSkipTaskbar = XInternAtom(dpy, "_NET_WM_STATE_SKIP_TASKBAR", False);
        Atom stateSkipPager = XInternAtom(dpy, "_NET_WM_STATE_SKIP_PAGER", False);
        Atom windowTypeOverlay = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_OVERLAY", False);
        

        Atom states[] = {stateAbove, stateFullscreen, stateSkipTaskbar, stateSkipPager, windowTypeOverlay};

        XChangeProperty(dpy, win, stateAtom, XA_ATOM, 32,
                    PropModeReplace, (unsigned char *)states, 4);

        XRaiseWindow(dpy, win);

        XGrabKeyboard(dpy, win, True, 
                    GrabModeAsync, GrabModeAsync, CurrentTime);

        XGrabPointer(dpy, win, True, 
                    ButtonPressMask | ButtonReleaseMask | PointerMotionMask,
                    GrabModeAsync, GrabModeAsync, None, None, CurrentTime);

        XFlush(dpy);
    }
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setDesktopFileName(OVRELAY_APP_ID);

    install_signal_handler();

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty(QStringLiteral("shutdownNotifier"), ShutdownNotifier::instance());

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