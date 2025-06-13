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
#include <iomanip>

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
    std::chrono::system_clock::time_point startTime;
    
    ProcessInfo(int p, const std::string& cmd, ProcessState s) 
        : pid(p), command(cmd), state(s), startTime(std::chrono::system_clock::now()) {}
   
};

std::string getRunningTime(const std::chrono::system_clock::time_point& start) {
    auto now = std::chrono::system_clock::now();
    auto duration = now - start;
    auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration % std::chrono::hours(1));
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration % std::chrono::minutes(1));
    
    std::ostringstream oss;
    oss << std::setfill('0') 
        << std::setw(2) << hours.count() << ":"
        << std::setw(2) << minutes.count() << ":"
        << std::setw(2) << seconds.count();
    return oss.str();
}

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
    
    // Calculate column widths
    const int pidWidth = 10;
    const int cmdWidth = 40;
    const int stateWidth = 15;
    const int timeWidth = 15;
    
    // Border characters
    const char vertical = '|';
    const char horizontal = '-';
    const char cross = '+';
    
    // Create horizontal line
    auto printHorizontalLine = [&]() {
        std::cout << cross 
                  << std::setfill(horizontal) << std::setw(pidWidth) << "" << cross
                  << std::setw(cmdWidth) << "" << cross
                  << std::setw(stateWidth) << "" << cross
                  << std::setw(timeWidth) << "" << cross
                  << std::setfill(' ') << "\n";
    };
    
    // Print table header
    std::cout << "\n";
    printHorizontalLine();
    
    std::cout << vertical << std::left << std::setw(pidWidth) << " PID" 
              << vertical << std::setw(cmdWidth) << " Command" 
              << vertical << std::setw(stateWidth) << " Status" 
              << vertical << std::setw(timeWidth) << " Runtime" 
              << vertical << "\n";
    
    printHorizontalLine();
    
    // Print process rows
    for (const auto& p : g_processes) {
        std::string truncatedCmd = p.command.length() > cmdWidth-2 ? 
                                 p.command.substr(0, cmdWidth-5) + "..." : p.command;
        std::string stateStr = (p.state == ProcessState::Running ? "Running" : "Suspended");
        
        std::cout << vertical << " " << std::left << std::setw(pidWidth-1) << p.pid
                  << vertical << " " << std::setw(cmdWidth-1) << truncatedCmd
                  << vertical << " " << std::setw(stateWidth-1) << stateStr
                  << vertical << " " << std::setw(timeWidth-1) << getRunningTime(p.startTime)
                  << vertical << "\n";
    }
    
    if (g_processes.empty()) {
        std::cout << vertical << " " << std::left << std::setw(pidWidth + cmdWidth + stateWidth + timeWidth + 3) 
                  << " No tracked processes " << vertical << "\n";
    }
    
    printHorizontalLine();
    std::cout << "\n";
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
    //g_processes.push_back({ (int)pi.dwProcessId, command, ProcessState::Running });
    g_processes.push_back({ 
    static_cast<int>(pi.dwProcessId), 
    command, 
    ProcessState::Running 
    });

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return true;
}



} // namespace process
