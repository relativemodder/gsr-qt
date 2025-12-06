#include "toggleshowaction.h"
#include <KGlobalAccel>

ToggleShowAction::ToggleShowAction(QObject *parent)
    : QAction{parent}
{
    setObjectName(QStringLiteral("toggleGsrOverlayShowAction"));
    setText(QStringLiteral("Toggle Show"));
    KGlobalAccel::self()->setDefaultShortcut(this, QList<QKeySequence>() << (Qt::ALT | Qt::Key_Z));
    KGlobalAccel::self()->setShortcut(this, QList<QKeySequence>() << (Qt::ALT | Qt::Key_Z));
}