#include "process.h"
#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <mutex>
#include <windows.h>
#include <tlhelp32.h>
#include <algorithm>

namespace process {

int foregroundPid = -1;
std::mutex processMutex;

void signalHandler(int signal) {
    if (signal == SIGINT && foregroundPid > 0) {
        std::cout << "Ctrl+C pressed. Attempting to terminate foreground process.\n";
        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, foregroundPid);
        if (hProcess) {
            TerminateProcess(hProcess, 0);
            CloseHandle(hProcess);
        }
        foregroundPid = -1;
    }
}

void setupCtrlCHandler() {
    std::signal(SIGINT, signalHandler);
}

enum class ProcessState { Running, Suspended };

struct ProcessInfo {
    int pid;
    std::string command;
    ProcessState state;
};

static std::vector<ProcessInfo> g_processes;

bool isProcessAlive(int pid) {
    HANDLE hProcess = OpenProcess(SYNCHRONIZE, FALSE, pid);
    if (!hProcess) return false;
    DWORD ret = WaitForSingleObject(hProcess, 0);
    CloseHandle(hProcess);
    return ret == WAIT_TIMEOUT;
}

void cleanupDeadProcesses() {
    std::lock_guard<std::mutex> lock(processMutex);
    g_processes.erase(std::remove_if(g_processes.begin(), g_processes.end(), [](const ProcessInfo& p) {
        return !isProcessAlive(p.pid);
    }), g_processes.end());
}

void listProcesses() {
    cleanupDeadProcesses();
    std::cout << "=== Running Processes ===\n";
    for (const auto& p : g_processes) {
        std::cout << "PID: " << p.pid << ", Command: " << p.command
                  << ", State: " << (p.state == ProcessState::Running ? "Running" : "Suspended") << "\n";
    }
    if (g_processes.empty()) {
        std::cout << "No tracked processes.\n";
    }
}

bool terminateProcess(int pid) {
    if (pid < 0) {
        std::cerr << "Invalid PID.\n";
        return false;
    }

    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (!hProcess) {
        std::cerr << "Failed to open process. Error code: " << GetLastError() << "\n";
        return false;
    }

    bool result = TerminateProcess(hProcess, 0);
    CloseHandle(hProcess);
    return result;
}

bool suspendProcess(int pid) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return false;

    THREADENTRY32 te;
    te.dwSize = sizeof(te);

    if (!Thread32First(hSnapshot, &te)) {
        CloseHandle(hSnapshot);
        return false;
    }

    do {
        if (te.th32OwnerProcessID == pid) {
            HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te.th32ThreadID);
            if (hThread) {
                SuspendThread(hThread);
                CloseHandle(hThread);
            }
        }
    } while (Thread32Next(hSnapshot, &te));

    CloseHandle(hSnapshot);

    for (auto& proc : g_processes) {
        if (proc.pid == pid) {
            proc.state = ProcessState::Suspended;
            break;
        }
    }

    return true;
}

bool resumeProcess(int pid) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return false;

    THREADENTRY32 te;
    te.dwSize = sizeof(te);

    if (!Thread32First(hSnapshot, &te)) {
        CloseHandle(hSnapshot);
        return false;
    }

    do {
        if (te.th32OwnerProcessID == pid) {
            HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te.th32ThreadID);
            if (hThread) {
                ResumeThread(hThread);
                CloseHandle(hThread);
            }
        }
    } while (Thread32Next(hSnapshot, &te));

    CloseHandle(hSnapshot);

    for (auto& proc : g_processes) {
        if (proc.pid == pid) {
            proc.state = ProcessState::Running;
            break;
        }
    }

    return true;
}

bool startForegroundProcess(const std::string& command) {
    setupCtrlCHandler();

    PROCESS_INFORMATION pi;
    STARTUPINFO si = { sizeof(STARTUPINFO) };
    std::string cmd = command;

    if (!CreateProcess(NULL, &cmd[0], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        
        return false;
    }

    foregroundPid = pi.dwProcessId;
    WaitForSingleObject(pi.hProcess, INFINITE);
    foregroundPid = -1;

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return true;
}

bool startBackgroundProcess(const std::string& command, bool addPause = false) {
    PROCESS_INFORMATION pi;
    STARTUPINFO si = { sizeof(STARTUPINFO) };

    std::string cmdPath = command;
    if (command.find(':') == std::string::npos && command[0] != '.' && command[0] != '\\' && command[0] != '/') {
        cmdPath = ".\\" + command;
    }
    for (char& c : cmdPath) if (c == '/') c = '\\';
    std::string fullCommand = cmdPath;
    if (addPause) fullCommand += " & pause";

    std::string cmd = fullCommand;

    if (!CreateProcess(NULL, &cmd[0], NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        
        return false;
    }

    std::cout << "Background process started with PID: " << pi.dwProcessId << "\n";

    std::lock_guard<std::mutex> lock(processMutex);
    g_processes.push_back({ (int)pi.dwProcessId, command, ProcessState::Running });

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return true;
}

// Utility functions
void runChild(bool is_background, std::string command) {
    std::string cmd = "programs/child.exe " + command;
    is_background ? startBackgroundProcess(cmd, true) : startForegroundProcess(cmd);
}

void runCountdown(bool is_background, int seconds) {
    std::string cmd = "programs/countdown.exe " + std::to_string(seconds);
    is_background ? startBackgroundProcess(cmd, true) : startForegroundProcess(cmd);
}

void runTicTacToe(bool is_background) {
    std::string cmd = "programs/tictactoe.exe";
    is_background ? startBackgroundProcess(cmd, true) : startForegroundProcess(cmd);
}

void runSpinner(bool is_background) {
    std::string cmd = "programs/spinner.exe";
    is_background ? startBackgroundProcess(cmd, false) : startForegroundProcess(cmd);
}

void runLogger(bool is_background) {
    std::string cmd = "programs/logger.exe";
    is_background ? startBackgroundProcess(cmd, false) : startForegroundProcess(cmd);
}

void runHeart(bool is_background) {
    std::string cmd = "programs/heart.exe";
    is_background ? startBackgroundProcess(cmd, false) : startForegroundProcess(cmd);
}
 void runDuck(bool is_background) {
    std::string cmd = "programs/duck.exe";
    is_background ? startBackgroundProcess(cmd, false) : startForegroundProcess(cmd);
 }

} // namespace process
