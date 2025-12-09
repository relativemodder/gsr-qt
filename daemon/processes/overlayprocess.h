#pragma once
#include <QObject>
#include <QAction>
#include <QProcess>

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
    QProcess* process = nullptr;
    bool alreadyTerminating = false;

public slots:
    void onStdOut();
    void onStdErr();
};