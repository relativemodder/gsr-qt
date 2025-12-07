#pragma once
#include <QObject>
#include <QAction>
#include <QProcess>

class InstanceProcess : public QObject 
{
    Q_OBJECT

public:
    explicit InstanceProcess(QObject *parent = nullptr);
    static InstanceProcess* instance();

    std::string getCurrentExecutablePath();
    std::vector<pid_t> findOtherInstancesPids();

    void subToSigUsr1();

public slots:
    void onSigUsr1();
};