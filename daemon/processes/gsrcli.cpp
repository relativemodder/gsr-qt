#include "gsrcli.h"
#include "gsrargs.h"
#include <csignal>
#include <gsrsettings.h>
#include <QStringList>
#include <iostream>
#include <qprocess.h>

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
    argsBuilder.addAudioSource("default_output|alsa_input.pci-0000_03_00.6.analog-stereo");
    argsBuilder.setOutputFile(GSRSettings::instance().getOutputDir() + "/testvideo.mp4");

    QStringList args;
    args << "gpu-screen-recorder";
    args << argsBuilder.toQStringList();

    recordProcess = new QProcess();
    recordProcess->start("/usr/bin/env", args);

    connect(recordProcess, &QProcess::finished,
            this,
            [this](int, QProcess::ExitStatus) {
                std::cerr << "[gsr] finished()\n";
                this->m_recording = false;
                emit recordingChanged();
            });

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

bool GSRCli::isRecording() {
    return m_recording;
}