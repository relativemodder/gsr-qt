#pragma once
#include <QObject>
#include <QAction>
#include <QProcess>

enum NotificationType {
    NORMAL = 0,
    ERROR = 1
};

class NotificationService : public QObject 
{
    Q_OBJECT

public:
    explicit NotificationService(QObject *parent = nullptr);
    static NotificationService* instance();

    void notify(QString iconName, QString text, NotificationType notificationType);
    void showPlasmaOSD(QString icon, QString text);

private:
    void showOwnNotification(QString icon, QString text, NotificationType notificationType);
};