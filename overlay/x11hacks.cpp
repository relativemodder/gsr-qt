#include "x11hacks.h"
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <QWindow>
#include <qcoreapplication.h>
#include <unistd.h>

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