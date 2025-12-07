#include <iostream>
#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include "shortcuts.h"
#include "signal.h"
#include "processes/instanceprocess.h"

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

    QApplication a(argc, argv);

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

    Shortcuts::instance()->initGlobalActions();

    return a.exec();
}