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
    QString generateFileName();

    QList<GSRCaptureOption> getCaptureOptions();

signals:
    void recordingChanged();

private:
    QProcess* recordProcess = nullptr;
    bool m_recording;
};