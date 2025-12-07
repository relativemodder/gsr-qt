#include "x11fs.h"
#include <iostream>


X11Fullscreen::X11Fullscreen(QObject *parent)
    : QObject{parent}
{}

X11Fullscreen* X11Fullscreen::instance()
{
    static X11Fullscreen p;
    return &p;
}

Window X11Fullscreen::getActiveWindow(Display* display) 
{
    Window root = DefaultRootWindow(display);
    Atom active_window_atom = XInternAtom(display, "_NET_ACTIVE_WINDOW", False);
    Atom actual_type;
    int actual_format;
    unsigned long nitems, bytes_after;
    unsigned char *prop = nullptr;
    Window active_window = None;

    if (XGetWindowProperty(display, root, active_window_atom, 0, 1, False, XA_WINDOW,
                           &actual_type, &actual_format, &nitems, &bytes_after, &prop) == Success) {
        if (prop != nullptr && nitems >= 1) {
            active_window = *reinterpret_cast<Window*>(prop);
        }
        if (prop != nullptr) {
            XFree(prop);
        }
    }
    return active_window;
}

bool X11Fullscreen::isFullscreen(Display* display, Window window) 
{
    if (window == None) {
        return false;
    }

    Atom wm_state_atom = XInternAtom(display, "_NET_WM_STATE", False);
    Atom fullscreen_atom = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);
    Atom actual_type;
    int actual_format;
    unsigned long nitems, bytes_after;
    unsigned char *prop = nullptr;
    bool is_full = false;

    if (XGetWindowProperty(display, window, wm_state_atom, 0, 1024, False, XA_ATOM,
                           &actual_type, &actual_format, &nitems, &bytes_after, &prop) == Success) {
        if (prop != nullptr && actual_type == XA_ATOM) {
            Atom *atoms = reinterpret_cast<Atom*>(prop);

            for (unsigned long i = 0; i < nitems; ++i) {
                if (atoms[i] == fullscreen_atom) {
                    is_full = true;
                    break;
                }
            }
        }
        if (prop != nullptr) {
            XFree(prop);
        }
    }
    return is_full;
}

bool X11Fullscreen::isFullscreenWindowInFocus() 
{
    Display *display = XOpenDisplay(nullptr);
    if (display == nullptr) {
        std::cerr << "Can't open X display." << std::endl;
        return false;
    }

    Window active_window = getActiveWindow(display);
    bool result = false;

    if (active_window != None) {
        result = isFullscreen(display, active_window);
    }

    XCloseDisplay(display);
    return result;
}