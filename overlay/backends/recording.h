#include <QObject>
#include <QQmlProperty>

class RecordingBackend : public QObject 
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(bool isActive READ recordingActive WRITE toggleRecording NOTIFY recordingActiveChanged)
    Q_PROPERTY(bool isPaused READ recordingPaused NOTIFY recordingPausedChanged)
public:
    explicit RecordingBackend(QObject *parent = nullptr);
    static RecordingBackend* instance();

    bool recordingActive();
    void subToChangesSignal();
    bool recordingPaused();

signals:
    void recordingActiveChanged();
    void recordingPausedChanged();

public slots:
    void toggleRecording(bool state);
    void onRecordingActiveChanged();
    void onRecordingPausedChanged();

    void togglePause();

    QList<QString> getCaptureOptions();
};