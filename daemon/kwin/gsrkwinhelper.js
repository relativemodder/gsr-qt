// call our daddy daemon to inform about new active window title
function sendNewActiveWindowTitle(title) {
    callDBus(
        "com.github.relative.gsrqt", "/",
        "com.github.relative.gsrqt",
        "setActiveWindowTitle", title
    );
}

// don't track multiple windows' captions unnecessarily, map is ideal here
const captionHandlers = new Map();


function subToChanges(client) {
    if (!client) return;
    if (captionHandlers.has(client)) return; // already tracking

    const handler = () => {
        sendNewActiveWindowTitle(client.caption || "");
    };

    captionHandlers.set(client, handler);
    client.captionChanged.connect(handler);
}

// on every focus
workspace.windowActivated.connect((client) => {
    if (!client) return;

    sendNewActiveWindowTitle(client.caption || "");
    subToChanges(client);
});

// on script load, send current active window title, just in case
if (workspace.activeWindow) {
    sendNewActiveWindowTitle(workspace.activeWindow.caption || "");
    subToChanges(workspace.activeWindow);
}
