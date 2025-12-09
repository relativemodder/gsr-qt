#include "gsrcli.h"
#include "gsrargs.h"
#include <csignal>
#include <gsrsettings.h>
#include <QStringList>
#include <iostream>
#include <qprocess.h>
#include "../notificationservice.h"

GSRCli::GSRCli(QObject *parent)
    : QObject{parent}
{}

GSRCli* GSRCli::instance()
{
    static GSRCli p;
    return &p;
}

void GSRCli::startRecording() 
{
    if (isRecording()) 
    {
        return;
    }

    GSRArgs argsBuilder;
    argsBuilder.setWindowTarget("screen");
    argsBuilder.setFrameRate(60);
    argsBuilder.addAudioSource("default_output|default_input");
    argsBuilder.setOutputFile(GSRSettings::instance().getOutputDir() + "/testvideo.mp4");

    QStringList args;
    args << "gpu-screen-recorder";
    args << argsBuilder.toQStringList();

    recordProcess = new QProcess();
    recordProcess->start("/usr/bin/env", args);

    connect(recordProcess, &QProcess::finished,
            this,
            [this](int code, QProcess::ExitStatus) {
                std::cerr << "[gsr] finished()\n";
                this->m_recording = false;
                emit recordingChanged();

                if (code == 0) 
                {
                    NotificationService::instance()->notify("document-save", "Recording saved", NotificationType::NORMAL);
                }
            });
    connect(recordProcess, &QProcess::readyReadStandardOutput,
    this, [this] {
        std::cout << "recordProcess gsr output: " << recordProcess->readAllStandardOutput().toStdString();
    });

    connect(recordProcess, &QProcess::readyReadStandardError,
    this, [this] {
        std::cerr << "recordProcess gsr stderr: " << recordProcess->readAllStandardError().toStdString();
    });

    NotificationService::instance()->notify("media-record", "Started recording", NotificationType::NORMAL);

    m_recording = true;
    emit recordingChanged();
}

void GSRCli::stopRecording()
{
    if (!isRecording()) 
    {
        return;
    }
    m_recording = false;
    kill(recordProcess->processId(), SIGINT);
}

void GSRCli::toggleRecording()
{
    if (isRecording()) 
    {
        stopRecording();
        return;
    }

    startRecording();
}

bool GSRCli::isRecording() 
{
    return m_recording;
}
