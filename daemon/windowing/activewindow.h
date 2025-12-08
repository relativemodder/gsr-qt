#pragma once
#include <QObject>

struct GameWindowInfo {
    QString title;
    bool isFullscreen;
};

class ActiveWindow : public QObject 
{
    Q_OBJECT

private:
    GameWindowInfo activeWindowInfo;

public:
    explicit ActiveWindow(QObject *parent = nullptr);
    static ActiveWindow* instance();

    void applyKWinHacks();

    void setActiveWindowInfo(const GameWindowInfo& info);
    GameWindowInfo info() const { return activeWindowInfo; }

};