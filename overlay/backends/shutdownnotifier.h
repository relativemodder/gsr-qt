#pragma once
#include <QObject>
#include <QTimer>

inline QTimer* globalShutdownTimer = nullptr;

class ShutdownNotifier : public QObject {
    Q_OBJECT
public:
    explicit ShutdownNotifier(QObject *parent = nullptr);
    static ShutdownNotifier* instance();

    Q_PROPERTY(bool isShuttingDown READ isShuttingDown NOTIFY startShutdownAnimation);
    bool isShuttingDown() const;

public slots:
    void intendedClose();

signals:
    void startShutdownAnimation();

private:
    QTimer *m_shutdownTimer;
    bool m_isShuttingDown = false;
};
