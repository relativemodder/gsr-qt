
workspace.windowActivated.connect((client) => {
    print(client)
    if (!client) return;

    print(client.caption)
    const title = client.caption || "";

    callDBus(
        "com.github.relative.gsrqt", "/",
        "com.github.relative.gsrqt",
        "setActiveWindowTitle", title
    );
});
