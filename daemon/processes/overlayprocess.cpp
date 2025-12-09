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

void OverlayProcess::toggleShow()
{
    if (lockToggleShow)
        return;

    lockToggleShow = true;
    QTimer::singleShot(600, this, [this]() {
        lockToggleShow = false;
    });

    if (!process) {
        startOverlay();
        return;
    }

    if (alreadyTerminating) {
        std::cerr << "hard kill\n";
        process->kill();   // immediate SIGKILL
        return;
    }

    alreadyTerminating = true;

    std::cerr << "graceful terminate\n";
    process->terminate();

    // if not dead in 600 ms just force kill
    QTimer::singleShot(600, this, [this]() {
        if (process && process->state() != QProcess::NotRunning) {
            std::cerr << "terminate timeout, killing\n";
            process->kill();
        }
    });
}


void OverlayProcess::startOverlay()
{
    // kill extra processes first
    auto pids = findOverlayProcesses();
    if (pids.size() > 1) {
        std::cerr << "Multiple overlay processes found\n";
        for (pid_t pid : pids) {
            std::cerr << " - killing PID: " << pid << "\n";
            kill(pid, SIGKILL);
        }
    }

    process = new QProcess(this);
    alreadyTerminating = false;

    // environment
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    bool isX11Fullscreen = X11Fullscreen::instance()->isFullscreenWindowInFocus();
    bool isFsInDE = ActiveWindow::instance()->info().isFullscreen;
    bool isHyprland = (env.value("XDG_SESSION_DESKTOP") == "Hyprland");

    bool useXcb = ((isX11Fullscreen || isFsInDE) && !isHyprland);

    if (useXcb)
        env.insert("QT_QPA_PLATFORM", "xcb");

    process->setProcessEnvironment(env);

    connect(process, &QProcess::readyReadStandardOutput,
            this, &OverlayProcess::onStdOut);

    connect(process, &QProcess::readyReadStandardError,
            this, &OverlayProcess::onStdErr);

    connect(process, &QProcess::finished,
            this,
            [this](int, QProcess::ExitStatus) {
                std::cerr << "[overlay] finished()\n";
                if (process) {
                    process->deleteLater();
                    process = nullptr;
                }
                alreadyTerminating = false;
            });


    process->start("/usr/bin/env", { "gsr-qt-overlay" });

    if (!process->waitForStarted(100)) {
        std::cerr << "[overlay] Failed to start overlay\n";
        process->deleteLater();
        process = nullptr;
    }
}

// kill not controlled overlay processes
void OverlayProcess::overkill()
{
    auto pids = findOverlayProcesses();
    for (pid_t pid : pids) {
        kill(pid, SIGKILL);
    }

    if (process) {
        process->kill();
    }
}

// scan for overlay procs just in case
QList<pid_t> OverlayProcess::findOverlayProcesses()
{
    QList<pid_t> found;

    QDir procDir("/proc");
    QStringList entries = procDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QString &entry : entries) {
        bool ok = false;
        pid_t pid = entry.toInt(&ok);
        if (!ok)
            continue;

        QString cmdlinePath = "/proc/" + entry + "/cmdline";
        QFile f(cmdlinePath);

        if (!f.open(QIODevice::ReadOnly))
            continue;

        QByteArray raw = f.readAll();
        QString cmdline = QString::fromUtf8(raw).replace('\0', ' ');

        if (cmdline.contains("gsr-qt-overlay"))
            found << pid;
    }

    return found;
}

// handle output
void OverlayProcess::onStdOut()
{
    std::cout << "overlay stdout: "
              << process->readAllStandardOutput().toStdString();
}

void OverlayProcess::onStdErr()
{
    std::cerr << "overlay stderr: "
              << process->readAllStandardError().toStdString();
}
