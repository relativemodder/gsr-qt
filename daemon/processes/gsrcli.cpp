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
#include "processes/overlayprocess.h"
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

    auto& settings = GSRSettings::instance();

    auto fileName = generateFileName();
    auto outDir = settings.get_recordOutputDirectory();
    auto firstStagePath = outDir + "/" + fileName;

    if (!QDir(outDir).exists())
    {
        if (!QDir().mkpath(outDir)) {
            NotificationService::instance()->notify("data-error", "Failed to create " + outDir + "directory.", NotificationType::ERROR);  
        }
    }

    auto captureOption = settings.get_recordCaptureOption();

    GSRArgs argsBuilder;
    argsBuilder.setWindowTarget(captureOption);
    argsBuilder.setFrameRate(60);
    argsBuilder.addAudioSource("default_output|default_input");
    argsBuilder.setOutputFile(firstStagePath);

    if (captureOption == "region") 
    {
        if (OverlayProcess::instance()->isActive()) {
            OverlayProcess::instance()->shutdownOverlay();
        }
        auto regionResult = selectRectScreenArea();
        if (regionResult.isEmpty()) 
        {
            return;
        }

        argsBuilder.setCaptureRegion(regionResult);
    }

    if (captureOption == "portal")
    {
        if (OverlayProcess::instance()->isActive()) {
            OverlayProcess::instance()->shutdownOverlay();
        }
    }

    QStringList args;
    args << "gpu-screen-recorder";
    args << argsBuilder.toQStringList();

    recordProcess = new QProcess();
    recordProcess->start("/usr/bin/env", args);

    connect(recordProcess, &QProcess::finished,
            this,
            [this, fileName, firstStagePath, &settings, outDir, captureOption](int code, QProcess::ExitStatus) {
                std::cerr << "[gsr] finished()\n";
                this->m_recording = false;
                emit recordingChanged();

                if (code != 0)
                {
                    return;
                }

                NotificationService::instance()->notify("document-save", "Saved recording from" + captureOptionToReadable(captureOption), NotificationType::NORMAL);

                if (settings.get_recordCategorizeByTitle())
                {
                    auto currentWindowTitle = ActiveWindow::instance()->info().title.replace('/', '_');
                    auto categorizedDir = outDir + "/" + currentWindowTitle;

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
            NotificationService::instance()->notify("data-error", QString::fromStdString(error_message), NotificationType::ERROR);
        }
    });

    NotificationService::instance()->notify(
        "media-record", 
        "Started recording" + captureOptionToReadable(captureOption), 
        NotificationType::NORMAL
    );

    m_recording = true;
    emit recordingChanged();
}

QString GSRCli::captureOptionToReadable(QString captureOption) 
{
    QString whatDoWeRecord;

    if (captureOption == "screen")
    {
        whatDoWeRecord = " this monitor";
    }
    else if (captureOption == "region")
    {
        whatDoWeRecord = " the selection";
    }
    else if (captureOption == "portal")
    {
        whatDoWeRecord = " Desktop Portal contents";
    }
    else if (captureOption.contains('-'))
    {
        whatDoWeRecord = " monitor " + captureOption;
    }

    return whatDoWeRecord;
}

QString GSRCli::selectRectScreenArea() 
{
    auto env = QProcessEnvironment::systemEnvironment();

    QStringList args;
    args << "slop";

    auto srscProcess = new QProcess(this);
    srscProcess->start("/usr/bin/env", args);

    NotificationService::instance()->notify("select", "Select an area", NotificationType::NORMAL);

    srscProcess->waitForFinished(90000);

    std::cout << "done" << std::endl;

    auto result = srscProcess->exitCode();

    auto stdout = srscProcess->readAllStandardOutput();
    auto stderr = srscProcess->readAllStandardError();

    if (result != 0) 
    {
        NotificationService::instance()->notify(
            "data-error", "Failed to select area: " + stderr.trimmed(), 
            NotificationType::ERROR);
        return "";
    }

    return stdout.trimmed();
}

QList<GSRCaptureOption> GSRCli::getCaptureOptions() 
{
    QStringList args;
    args << "gpu-screen-recorder";
    args << "--list-capture-options";

    auto gcoProcess = new QProcess();
    gcoProcess->start("/usr/bin/env", args);

    gcoProcess->waitForFinished();

    auto result = gcoProcess->exitCode();

    auto stdout = gcoProcess->readAllStandardOutput();
    auto stderr = gcoProcess->readAllStandardError();

    QList<GSRCaptureOption> list = {{"screen"}};

    gcoProcess->deleteLater();

    if (result != 0) 
    {
        NotificationService::instance()->notify("data-error", "Failed to get capture options: " + stderr.trimmed(), NotificationType::ERROR);
        return list;
    }

    for (QString line : stdout.split('\n')) 
    {
        if (line.trimmed().size() < 1) continue;

        auto data = line.split('|');

        GSRCaptureOption option = {};
        option.name = data[0];

        if (data.length() > 1)
        {
            option.resolution = data[1];
        }

        list.append(option);
    }

    return list;
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
    m_recordingPaused = false;
    emit recordingPausedChanged();
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


bool GSRCli::isRecordingPaused()
{
    return m_recordingPaused;
}

void GSRCli::toggleRecordingPause()
{
    if (!isRecording()) 
    {
        return;
    }

    kill(recordProcess->processId(), SIGUSR2);
    m_recordingPaused = !m_recordingPaused;
    emit recordingPausedChanged();

    NotificationService::instance()->notify(
        "media-record", 
        QStringLiteral("Recording has been ") + (m_recordingPaused ? "paused" : "unpaused"), 
        NotificationType::NORMAL
    );
}