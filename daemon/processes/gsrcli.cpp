#include "gsrcli.h"
#include "gsrargs.h"
#include <csignal>
#include <gsrsettings.h>
#include <QStringList>
#include <iostream>
#include <qcontainerfwd.h>
#include <qcoreapplication.h>
#include <qdir.h>
#include <qprocess.h>
#include <QDateTime>
#include "../notificationservice.h"
#include "../windowing/activewindow.h"
#include <QFile>

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

    auto fileName = generateFileName();
    auto firstStagePath = GSRSettings::instance().getOutputDir() + "/" + fileName;

    GSRArgs argsBuilder;
    argsBuilder.setWindowTarget("screen");
    argsBuilder.setFrameRate(60);
    argsBuilder.addAudioSource("default_output|default_input");
    argsBuilder.setOutputFile(firstStagePath);

    QStringList args;
    args << "gpu-screen-recorder";
    args << argsBuilder.toQStringList();

    recordProcess = new QProcess();
    recordProcess->start("/usr/bin/env", args);

    connect(recordProcess, &QProcess::finished,
            this,
            [this, fileName, firstStagePath](int code, QProcess::ExitStatus) {
                std::cerr << "[gsr] finished()\n";
                this->m_recording = false;
                emit recordingChanged();

                if (code != 0)
                {
                    return;
                }

                NotificationService::instance()->notify("document-save", "Recording saved", NotificationType::NORMAL);

                if (GSRSettings::instance().getCategorizeByTitle())
                {
                    auto currentWindowTitle = ActiveWindow::instance()->info().title.replace('/', '_');
                    auto categorizedDir = GSRSettings::instance().getOutputDir() + "/" + currentWindowTitle;

                    if (!QDir(categorizedDir).exists()) {
                        QDir().mkdir(categorizedDir);
                    }

                    auto categorizedPath = categorizedDir + "/" + fileName;

                    std::cout << "Moving " << firstStagePath.toStdString() << " to " << categorizedPath.toStdString() << std::endl;

                    QFile::rename(firstStagePath, categorizedPath);
                }
            
            });
    connect(recordProcess, &QProcess::readyReadStandardOutput,
    this, [this] {
        std::cout << "recordProcess gsr output: " << recordProcess->readAllStandardOutput().toStdString();
    });

    connect(recordProcess, &QProcess::readyReadStandardError,
    this, [this] {
        auto stderr = recordProcess->readAllStandardError();
        std::cerr << "recordProcess gsr stderr: " << stderr.toStdString();
        if (stderr.contains("gsr error:")) 
        {
            std::string gsr_error_content = stderr.split(':').last().trimmed().toStdString();
            std::string error_message = "Error saving recording: "+ gsr_error_content;
            NotificationService::instance()->notify("data-error", QString::fromStdString(error_message), NotificationType::NORMAL);
        }
    });

    NotificationService::instance()->notify("media-record", "Started recording", NotificationType::NORMAL);

    m_recording = true;
    emit recordingChanged();
}

QString GSRCli::generateFileName()
{
    QString filenameBuilder;
    filenameBuilder.append("Video_");
    filenameBuilder.append(QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss"));
    filenameBuilder.append(".mp4");
    
    return filenameBuilder;
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
