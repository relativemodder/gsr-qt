#include "backends/shutdownnotifier.h"
#include <unistd.h>

ShutdownNotifier* ShutdownNotifier::instance()
{
    static ShutdownNotifier p;
    return &p;
}

void ShutdownNotifier::intendedClose() {
    m_isShuttingDown = true;
    emit startShutdownAnimation();
    m_shutdownTimer->start();
}

ShutdownNotifier::ShutdownNotifier(QObject *parent) {
    m_shutdownTimer = new QTimer(this);
    m_shutdownTimer->setSingleShot(true);
    m_shutdownTimer->setInterval(400);
    
    connect(m_shutdownTimer, &QTimer::timeout, [] {
        _exit(0);
    });
    
    globalShutdownTimer = m_shutdownTimer;
}

bool ShutdownNotifier::isShuttingDown() const {
    return m_isShuttingDown;
}
