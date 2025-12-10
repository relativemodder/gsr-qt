#include <QObject>
#include <QQmlProperty>

class RecordingBackend : public QObject 
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(bool isActive READ recordingActive WRITE toggleRecording NOTIFY recordingActiveChanged)
public:
    explicit RecordingBackend(QObject *parent = nullptr);
    static RecordingBackend* instance();

    bool recordingActive();
    void subToChangesSignal();

signals:
    void recordingActiveChanged();

public slots:
    void toggleRecording(bool state);
    void onRecordingActiveChanged();

    QList<QString> getCaptureOptions();
};