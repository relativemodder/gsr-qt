#pragma once
#include <QObject>
#include <QTimer>

inline QTimer* globalShutdownTimer = nullptr;

class ShutdownNotifier : public QObject {
    Q_OBJECT
public:
    explicit ShutdownNotifier(QObject *parent = nullptr);
    static ShutdownNotifier* instance();

public slots:
    void intendedClose();

signals:
    void startShutdownAnimation(); 

private:
    QTimer *m_shutdownTimer;
};