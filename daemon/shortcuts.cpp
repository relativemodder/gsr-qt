#include "shortcuts.h"
#include "actions/togglerecordaction.h"
#include "actions/toggleshowaction.h"
#include "processes/overlayprocess.h"
#include "processes/gsrcli.h"

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
    
    toggleRecord = new ToggleRecordAction();
    connect(toggleRecord, &QAction::triggered, this, &Shortcuts::onToggleRecordShortcutTriggered);
}

void Shortcuts::onToggleShowShortcutTriggered() 
{
    OverlayProcess::instance()->toggleShow();
}

void Shortcuts::onToggleRecordShortcutTriggered()
{
    auto gsr = GSRCli::instance();
    gsr->toggleRecording();
}