#include "notificationservice.h"
#include "dbusinterface.h"
#include <QDBusInterface>
#include <cstdlib>
#include <QtEnvironmentVariables>
#include <QStringList>
#include <iostream>

NotificationService::NotificationService(QObject *parent)
    : QObject{parent}
{}

NotificationService* NotificationService::instance()
{
    static NotificationService p;
    return &p;
}

void NotificationService::notify(QString iconName, QString text, NotificationType notificationType) 
{
    showOwnNotification(iconName, text, notificationType);
}

void NotificationService::showOwnNotification(QString icon, QString text, NotificationType notificationType) 
{
    QProcess* notificationProcess = new QProcess();
    QStringList args;
    args << "gsr-qt-notification";

    args << "--icon" << icon;
    args << "--text" << text;
    args << "--type" << (notificationType == NotificationType::ERROR ? "error" : "normal");

    connect(notificationProcess, &QProcess::finished,
        this,
        [&](int code, QProcess::ExitStatus) {
            std::cerr << "[notification] finished\n";
        });

    connect(notificationProcess, &QProcess::readyReadStandardOutput,
    this, [&] {
        std::cout << "notification output: " << notificationProcess->readAllStandardOutput().toStdString();
    });

    connect(notificationProcess, &QProcess::readyReadStandardError,
    this, [&] {
        std::cerr << "notification stderr: " << notificationProcess->readAllStandardError().toStdString();
    });

    notificationProcess->start("/usr/bin/env", args);
}

void NotificationService::showPlasmaOSD(QString icon, QString text) 
{
    auto connection = DBusInterface::instance()->getConnection();
    QDBusInterface iface(
        "org.freedesktop.Notifications", 
        "/org/kde/osdService", "org.kde.osdService", 
        *connection);
    
    iface.call("showText", icon, text);
}