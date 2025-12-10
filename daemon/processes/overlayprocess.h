#pragma once

#include <QObject>
#include <QProcess>
#include <QList>

class OverlayProcess : public QObject
{
    Q_OBJECT

public:
    explicit OverlayProcess(QObject *parent = nullptr);
    static OverlayProcess* instance();

    void toggleShow();
    QList<pid_t> findOverlayProcesses();
    void overkill();
    bool isActive();
    void shutdownOverlay(bool hardkill = false);
    void startOverlay();

private:
    QProcess* process = nullptr;
    bool alreadyTerminating = false;
    bool lockToggleShow = false;

public slots:
    void onStdOut();
    void onStdErr();
};
