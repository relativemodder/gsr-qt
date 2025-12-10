#pragma once
#include <QObject>
#include <QAction>
#include <QProcess>
#include <QString>
#include <QList>
#include <optional>

struct GSRCaptureOption {
    QString name;
    std::optional<QString> resolution;
};

class GSRCli : public QObject 
{
    Q_OBJECT

public:
    explicit GSRCli(QObject *parent = nullptr);
    static GSRCli* instance();

    void startRecording();
    void stopRecording();
    bool isRecording();
    void toggleRecording();
    bool isRecordingPaused();
    void toggleRecordingPause();
    QString generateFileName();

    QList<GSRCaptureOption> getCaptureOptions();
    QString selectRectScreenArea();
    QString captureOptionToReadable(QString captureOption);

signals:
    void recordingChanged();
    void recordingPausedChanged();

private:
    QProcess* recordProcess = nullptr;
    bool m_recording;
    bool m_recordingPaused;
};