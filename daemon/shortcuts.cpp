#include "shortcuts.h"
#include "actions/toggleshowaction.h"
#include "processes/overlayprocess.h"

Shortcuts::Shortcuts(QObject *parent)
    : QObject{parent}
{}

Shortcuts* Shortcuts::instance()
{
    static Shortcuts p;
    return &p;
}

void Shortcuts::initGlobalActions() 
{
    toggleShow = new ToggleShowAction();
    connect(toggleShow, &QAction::triggered, this, &Shortcuts::onToggleShowShortcutTriggered);
}

void Shortcuts::onToggleShowShortcutTriggered() 
{
    OverlayProcess::instance()->toggleShow();
}