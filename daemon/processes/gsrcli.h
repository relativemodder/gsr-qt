#pragma once
#include <QObject>
#include <QAction>
#include <QProcess>

class GSRCli : public QObject 
{
    Q_OBJECT

public:
    explicit GSRCli(QObject *parent = nullptr);
    static GSRCli* instance();

    void startRecording();
    void stopRecording();
    bool isRecording();

signals:
    void recordingChanged();

private:
    QProcess* recordProcess = nullptr;
    bool m_recording;
};