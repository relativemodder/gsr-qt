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

private:
    QProcess* process = nullptr;

public slots:
    void onStdOut();
    void onStdErr();
};