# GPU Screen Recorder (Qt frontend)

Shadowplay-like overlay for [GPU Screen Recorder](https://git.dec05eba.com/gpu-screen-recorder/) by dec05eba.

<img src="/assets/thumbnail.png">

Made in C++/QML.

Not licensed yet, but I guess that it needs to be GPL v3, as the original project is.


**GSR Qt is available in two modes:**
- Wayland (layer-shell)
- X11 (not implemented correctly yet) (and also this mode needs to be enabled automatically when X11 full-screen game is in focus)


## How to build
You need Qt6, LayerShellQt, X11, CMake, KDE CMake extras and KDE Frameworks installed on your system.


```bash
mkdir build && cd build
cmake -D CMAKE_BUILD_TYPE=Release ..
cmake --build . -- -j 8
```

## How to run

```bash
sudo ./install.sh
gsr-qt-daemon-service # Only tested on KDE Plasma, idk how KGlobalAccel works on other DEs
```

### Toggle the overlay
Press ALT+Z, if you're on KDE Plasma (check out plasmasettings -> Keyboard -> Shortcuts)


```bash
killall -s 10 gsr-qt-daemon-service # signal way

...

/usr/lib/qt6/bin/qdbus com.github.relative.gsrqt / com.github.relative.gsrqt.toggleShow # DBus way

...

gsr-qt-daemon-service # it just sends the signal
```

## Why do my keyboard presses don't work when I'm in game?
The overlay has to fall through your presses. If it enables the interactivity while the game is in fullscreen mode, there's a high chance that it will minimize. Don't worry, global shortcuts like Alt+Z and Alt+F9 still work, it's just the overlay UI in-game.