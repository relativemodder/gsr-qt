#pragma once
#include <QObject>
#include <QAction>
#include <QProcess>
#include "actions/toggleshowaction.h"

class Shortcuts : public QObject 
{
    Q_OBJECT

public:
    explicit Shortcuts(QObject *parent = nullptr);
    static Shortcuts* instance();

    void initGlobalActions();

private:
    
    ToggleShowAction* toggleShow;

public slots:
    void onToggleShowShortcutTriggered();
};