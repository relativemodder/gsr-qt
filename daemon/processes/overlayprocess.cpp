#include "overlayprocess.h"
#include <iostream>
#include <QTimer>
#include <QDir>
#include <QFile>
#include <signal.h>
#include "../windowing/x11fs.h"
#include "../windowing/activewindow.h"

OverlayProcess::OverlayProcess(QObject *parent)
    : QObject{parent}
{}

OverlayProcess* OverlayProcess::instance()
{
    static OverlayProcess p;
    return &p;
}

void OverlayProcess::toggleShow() {
    QString program = "/usr/bin/env";
    QStringList arguments;

    arguments << "gsr-qt-overlay";

    if (findOverlayProcesses().count() > 1) {
        std::cerr << "Multiple overlay processes detected, overkilling.\n";
        for (pid_t pid : findOverlayProcesses()) {
            std::cerr << " Killing PID: " << pid << std::endl;
            kill(pid, SIGKILL);
        }
        delete process;
        process = nullptr;
        alreadyTerminating = false;
    }

    if (process == nullptr) {
        process = new QProcess(this);
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

        auto currentDesktop = env.value("XDG_SESSION_DESKTOP", "wtf");

        bool isX11Fullscreen = X11Fullscreen::instance()->isFullscreenWindowInFocus();
        bool isFsInDE = ActiveWindow::instance()->info().isFullscreen;
        bool isHyprland = (currentDesktop == "Hyprland");

        bool considerXcb = (isX11Fullscreen && !isHyprland) || (isFsInDE && !isHyprland);

        if (considerXcb) {
            if (env.contains("QT_QPA_PLATFORM")) {
                env.remove("QT_QPA_PLATFORM");
            }

            env.insert("QT_QPA_PLATFORM", "xcb");
        }

        process->setProcessEnvironment(env);

        process->start(program, arguments);

        connect(process, &QProcess::readyReadStandardOutput, this, &OverlayProcess::onStdOut);
        connect(process, &QProcess::readyReadStandardError, this, &OverlayProcess::onStdErr);

        connect(process, &QProcess::finished, [&](int exitCode, QProcess::ExitStatus exitStatus) {
            delete process;
            process = nullptr;
        });

        alreadyTerminating = false;
    }
    else {
        if (alreadyTerminating) {
            overkill();
            return;
        }

        alreadyTerminating = true;
        
        overkill();

        if (process->state() == QProcess::Running) {
            kill(process->processId(), SIGINT);
        }
        else {
            delete process;
            process = nullptr;

            return;
        }
    }
}

void OverlayProcess::overkill()
{
    if (findOverlayProcesses().isEmpty()) {
        delete process;
        process = nullptr;
        alreadyTerminating = false;
    }
}

QList<pid_t> OverlayProcess::findOverlayProcesses()
{
    QList<pid_t> found;

    QDir procDir("/proc");
    QStringList entries = procDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QString &entry : entries) {
        bool ok = false;
        pid_t pid = entry.toInt(&ok);
        if (!ok) continue;

        QString cmdlinePath = "/proc/" + entry + "/cmdline";
        QFile f(cmdlinePath);
        if (!f.open(QIODevice::ReadOnly))
            continue;

        QByteArray raw = f.readAll();
        QString cmd = QString::fromUtf8(raw).replace('\0', ' ').trimmed();

        if (cmd.contains("gsr-qt-overlay")) {
            found << pid;
        }
    }

    return found;
}

void OverlayProcess::onStdOut() 
{
    QByteArray output = process->readAllStandardOutput();
    std::cout << "overlay stdout: " << output.toStdString();
}

void OverlayProcess::onStdErr() 
{
    QByteArray output = process->readAllStandardError();
    std::cerr << "overlay stderr: " << output.toStdString();
}