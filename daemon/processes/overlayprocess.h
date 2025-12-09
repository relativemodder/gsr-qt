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

private:
    void startOverlay();

    QProcess* process = nullptr;
    bool alreadyTerminating = false;

public slots:
    void onStdOut();
    void onStdErr();
};
