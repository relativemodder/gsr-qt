#pragma once
#include <QObject>
#include <QAction>
#include <QProcess>
#include "actions/toggleshowaction.h"
#include "actions/togglerecordaction.h"
#include "actions/togglerecordpauseaction.h"

class Shortcuts : public QObject 
{
    Q_OBJECT

public:
    explicit Shortcuts(QObject *parent = nullptr);
    static Shortcuts* instance();

    void initGlobalActions();

private:
    ToggleShowAction* toggleShow;
    ToggleRecordAction* toggleRecord;
    ToggleRecordPauseAction* toggleRecordPause;

public slots:
    void onToggleShowShortcutTriggered();
    void onToggleRecordShortcutTriggered();
    void onToggleRecordPauseShortcutTriggered();
};