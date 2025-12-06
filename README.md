# GPU Screen Recorder (Qt frontend)

Shadowplay-like overlay for [GPU Screen Recorder](https://git.dec05eba.com/gpu-screen-recorder/) by dec05eba.

Made in C++/QML.

Not licensed yet, but I guess that it needs to be GPL v3, as the original project is.


**GSR Qt is available in two modes:**
- Wayland (layer-shell)
- X11 (not implemented correctly yet) (and also this mode needs to be enabled automatically when X11 full-screen game is in focus)


## How to build
You need Qt6, X11, CMake, KDE CMake extras and KDE Frameworks installed on your system.


```bash
mkdir build && cd build
cmake -D CMAKE_BUILD_TYPE=Release ..
cmake --build . -- -j 8
```

## How to run

```bash
cd build
export PATH=$(pwd)/overlay

./bin/gsr-qt-daemon-service # Only tested on KDE Plasma, idk how KGlobalAccel works on other DEs
```

### Run only GUI, without daemon
```bash
cd build/overlay
./gsr-qt-overlay
```


<img src="/assets/thumbnail.png">