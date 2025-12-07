#include "instanceprocess.h"
#include <QTimer>
#include <iostream>
#include <unistd.h>
#include <dirent.h>
#include <csignal>
#include <signal.h>
#include "overlayprocess.h"

InstanceProcess::InstanceProcess(QObject *parent)
    : QObject{parent}
{}

InstanceProcess* InstanceProcess::instance()
{
    static InstanceProcess p;
    return &p;
}

std::string InstanceProcess::getCurrentExecutablePath() 
{
    char path[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if (len != -1) {
        path[len] = '\0';
        return std::string(path);
    }
    return "";
}

std::vector<pid_t> InstanceProcess::findOtherInstancesPids() 
{
    std::vector<pid_t> pids;
    std::string current_exe_path = getCurrentExecutablePath();
    if (current_exe_path.empty()) return pids;

    pid_t current_pid = getpid();
    DIR* dir = opendir("/proc");
    if (!dir) return pids;

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type == DT_DIR) {
            bool is_digit = true;
            for (char* p = entry->d_name; *p; ++p) {
                if (!isdigit(*p)) {
                    is_digit = false;
                    break;
                }
            }
            if (is_digit) {
                pid_t pid = std::stoul(entry->d_name);
                if (pid == current_pid) continue;

                std::string exe_symlink = "/proc/" + std::to_string(pid) + "/exe";
                char exe_path_buffer[PATH_MAX];
                ssize_t len = readlink(exe_symlink.c_str(), exe_path_buffer, sizeof(exe_path_buffer) - 1);

                if (len != -1) {
                    exe_path_buffer[len] = '\0';
                    std::string process_exe_path(exe_path_buffer);
                    if (process_exe_path == current_exe_path) {
                        pids.push_back(pid);
                    }
                }
            }
        }
    }
    closedir(dir);
    return pids;
}

void InstanceProcess::onSigUsr1() 
{
    OverlayProcess::instance()->toggleShow();
}

void sigusr_signal_handler(int signum) 
{
    std::cout << "Handling SIGUSR\n";
    emit InstanceProcess::instance()->onSigUsr1();
}

void InstanceProcess::subToSigUsr1() 
{
    struct sigaction sa;
    sa.sa_handler = sigusr_signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        std::cerr << "Cannot install SIGINT handler!\n";
    }
}