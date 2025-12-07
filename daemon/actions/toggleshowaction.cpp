#include "toggleshowaction.h"
#include <KGlobalAccel>
#include <QProcess>

ToggleShowAction::ToggleShowAction(QObject *parent)
    : QAction{parent}
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    auto currentDesktop = env.value("XDG_SESSION_DESKTOP", "wtf");

    if (currentDesktop == "KDE") {
        setObjectName(QStringLiteral("toggleGsrOverlayShowAction"));
        setText(QStringLiteral("Toggle Show"));
        KGlobalAccel::self()->setDefaultShortcut(this, QList<QKeySequence>() << (Qt::ALT | Qt::Key_Z));
        KGlobalAccel::self()->setShortcut(this, QList<QKeySequence>() << (Qt::ALT | Qt::Key_Z));
    }
}