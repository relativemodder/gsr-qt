#pragma once
#include <QObject>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

class X11Fullscreen : public QObject 
{
    Q_OBJECT

public:
    explicit X11Fullscreen(QObject *parent = nullptr);
    static X11Fullscreen* instance();

    Window getActiveWindow(Display* display);
    bool isFullscreen(Display* display, Window window);
    bool isFullscreenWindowInFocus();

};