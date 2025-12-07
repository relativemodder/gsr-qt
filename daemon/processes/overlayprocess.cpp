#include "overlayprocess.h"
#include <iostream>
#include <QTimer>
#include <signal.h>
#include "../windowing/x11fs.h"

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

    static bool alreadyTerminating = false;

    if (process == nullptr) {
        process = new QProcess(this);
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

        if (X11Fullscreen::instance()->isFullscreenWindowInFocus()) {
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
            return;
        }

        alreadyTerminating = true;

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