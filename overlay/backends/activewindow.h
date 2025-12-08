#include <QObject>
#include <QQmlProperty>

class ActiveWindow : public QObject 
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString title READ getTitle WRITE setTitle NOTIFY titleChanged)
public:
    explicit ActiveWindow(QObject *parent = nullptr);
    static ActiveWindow* instance();

    QString getTitle() const;
    void setTitle(const QString& t);
    void requestWindowTitle();

signals:
    void titleChanged();

private:
    QString m_title;
};