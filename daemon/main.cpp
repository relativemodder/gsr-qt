#include <iostream>
#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QDBusConnection>
#include <QDBusError>
#include "shortcuts.h"
#include "signal.h"
#include "processes/instanceprocess.h"
#include "windowing/activewindow.h"
#include "dbusinterface.h"

int main(int argc, char *argv[])
{
    std::vector<pid_t> running_pids = InstanceProcess::instance()->findOtherInstancesPids();

    if (!running_pids.empty()) {
        for (pid_t pid : running_pids) {
            std::cout << " Sending SIGUSR1 to PID: " << pid << std::endl;
            
            if (kill(pid, SIGUSR1) == 0) {
                std::cout << "Successful.\n";
            } else {
                std::cerr << "Error sending SIGUSR1\n"; 
                exit(-1);
            }
        }
        
        exit(0);
    }

    std::cout << " No other instance is running. Starting daemon...\n";

    QApplication a(argc, argv);

    auto connection = QDBusConnection::sessionBus();

    if (!connection.isConnected()) {
        std::cerr << "Cannot connect to the D-Bus session bus.\n"
                 "To start it, run:\n"
                 "\teval `dbus-launch --auto-syntax`\n";
        return 1;
    }

    if (!connection.registerService(APP_ID)) {
        qWarning("%s\n", qPrintable(connection.lastError().message()));
        exit(1);
    }
    
    DBusInterface::instance()->setConnection(&connection);

    connection.registerObject("/", DBusInterface::instance(), QDBusConnection::ExportAllSlots);

    InstanceProcess::instance()->subToSigUsr1();

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    auto currentDesktop = env.value("XDG_SESSION_DESKTOP", "wtf");

    if (currentDesktop != "KDE") {
        QMessageBox::warning(
            nullptr, 
            "Desktop is not supported", 
            currentDesktop + " is not supported by now. KDE is recommended.\n\nIf you want to open the overlay, call gsr-qt-daemon-service again or send SIGUSR1 to it."
        );
    }
    else {
        ActiveWindow::instance()->applyKWinHacks();
    }

    Shortcuts::instance()->initGlobalActions();


    return a.exec();
}