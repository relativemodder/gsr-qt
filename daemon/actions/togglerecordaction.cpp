#include "togglerecordaction.h"
#include <KGlobalAccel>
#include <QProcess>

ToggleRecordAction::ToggleRecordAction(QObject *parent)
    : QAction{parent}
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    auto currentDesktop = env.value("XDG_SESSION_DESKTOP", "wtf");

    if (currentDesktop == "KDE") {
        setObjectName(QStringLiteral("toggleGsrRecordAction"));
        setText(QStringLiteral("Start/stop the recording"));
        KGlobalAccel::self()->setDefaultShortcut(this, QList<QKeySequence>() << (Qt::ALT | Qt::Key_F9));
        KGlobalAccel::self()->setShortcut(this, QList<QKeySequence>() << (Qt::ALT | Qt::Key_F9));
    }
}