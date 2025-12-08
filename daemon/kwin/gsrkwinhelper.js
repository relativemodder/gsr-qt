// call our daddy daemon to inform about new active window title
function sendNewActiveWindowTitle(title) {
    callDBus(
        "com.github.relative.gsrqt", "/",
        "com.github.relative.gsrqt",
        "setActiveWindowTitle", title
    );
}

function sendNewActiveWindowFullscreen(isFullscreen) {
    callDBus(
        "com.github.relative.gsrqt", "/",
        "com.github.relative.gsrqt",
        "setActiveWindowFullscreen", isFullscreen
    );
}

// don't track multiple windows' captions unnecessarily, map is ideal here
const captionHandlers = new Map();


function subToChanges(client) {
    if (!client) return;
    if (captionHandlers.has(client)) return; // already tracking

    const handler = () => {
        if (workspace.activeWindow === client) {
            sendNewActiveWindowTitle(client.caption || "");
        }
    };

    const fsHandler = () => {
        if (workspace.activeWindow === client) {
            sendNewActiveWindowFullscreen(client.fullScreen);
        }
    }

    captionHandlers.set(client, handler);
    client.captionChanged.connect(handler);
    client.fullScreenChanged.connect(fsHandler);
}

// on every focus
workspace.windowActivated.connect((client) => {
    if (!client) return;

    sendNewActiveWindowTitle(client.caption || "");
    sendNewActiveWindowFullscreen(client.fullScreen);
    subToChanges(client);
});

// on script load, send current active window title, just in case
if (workspace.activeWindow) {
    sendNewActiveWindowTitle(workspace.activeWindow.caption || "");
    sendNewActiveWindowFullscreen(workspace.activeWindow.fullScreen);
    subToChanges(workspace.activeWindow);
}
