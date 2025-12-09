#include <QGuiApplication>
#include <QObject>
#include <QQuickWindow>
#include <QQmlApplicationEngine>
#include <QMargins>
#include <QCommandLineParser>
#include <QQmlContext>
#include <qnamespace.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("Notification bubble");
    parser.addHelpOption();

    QCommandLineOption iconOpt("icon", "Icon name", "icon", "document-save");
    QCommandLineOption textOpt("text", "Message text", "text", "Recording saved!");
    QCommandLineOption timeoutOpt("timeout", "Reading speed multiplier", "ms", "40");
    QCommandLineOption typeOpt("type", "Type of the notification ('normal', 'error')", "type", "normal");

    parser.addOption(iconOpt);
    parser.addOption(textOpt);
    parser.addOption(timeoutOpt);
    parser.addOption(typeOpt);

    parser.process(app);

    QString icon = parser.value(iconOpt);
    QString text = parser.value(textOpt);
    int readSpeed = parser.value(timeoutOpt).toInt();
    QString notificationType = parser.value(typeOpt);

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("backendIcon", icon);
    engine.rootContext()->setContextProperty("backendText", text);
    engine.rootContext()->setContextProperty("backendReadSpeed", readSpeed);
    engine.rootContext()->setContextProperty("backendNotificationType", notificationType);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [](QObject *obj, const QUrl &objUrl) {
        QQuickWindow *window = qobject_cast<QQuickWindow*>(obj);
        window->setFlags(Qt::WindowTransparentForInput);
        window->show();
    }, Qt::QueuedConnection);

    engine.loadFromModule("notification", "Notification");

    return app.exec();
}
