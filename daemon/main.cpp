#include <iostream>
#include <QApplication>
#include "shortcuts.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Shortcuts::instance()->initGlobalActions();

    return a.exec();
}