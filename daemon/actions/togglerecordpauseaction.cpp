#include "togglerecordpauseaction.h"
#include <KGlobalAccel>
#include <QProcess>

ToggleRecordPauseAction::ToggleRecordPauseAction(QObject *parent)
    : QAction{parent}
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    auto currentDesktop = env.value("XDG_SESSION_DESKTOP", "wtf");

    if (currentDesktop == "KDE") {
        setObjectName(QStringLiteral("toggleGsrRecordPauseAction"));
        setText(QStringLiteral("Pause/unpause the recording"));
        KGlobalAccel::self()->setDefaultShortcut(this, QList<QKeySequence>() << (Qt::ALT | Qt::Key_F10));
        KGlobalAccel::self()->setShortcut(this, QList<QKeySequence>() << (Qt::ALT | Qt::Key_F10));
    }
}