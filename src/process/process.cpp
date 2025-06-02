#include "process.h"
#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#ifdef _WIN32
#include <windows.h>
#include <tlhelp32.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#endif

namespace process {

    
        // Lưu PID của tiến trình foreground để có thể gửi tín hiệu
        int foregroundPid = -1;

        void signalHandler(int signal) {
            if (signal == SIGINT && foregroundPid > 0) {
#ifdef _WIN32
               
                std::cout << "Ctrl+C pressed. Terminating foreground process (manual support needed on Windows).\n";
#else
                kill(foregroundPid, SIGTERM);
                std::cout << "\nForeground process terminated.\n";
#endif
                foregroundPid = -1;
            }
        }
    

    void setupCtrlCHandler() {
        std::signal(SIGINT, process::signalHandler);
    }

    struct ProcessInfo {
        int pid;
        std::string command;
    };

    static std::vector<ProcessInfo> g_processes;

    void listProcesses() {
        std::cout << "=== Running Processes ===\n";
        for (const auto& p : g_processes) {
            std::cout << "PID: " << p.pid << ", Command: " << p.command << "\n";
        }
        if (g_processes.empty()) {
            std::cout << "No tracked processes.\n";
        }
    }

    bool terminateProcess(int pid) {
#ifdef _WIN32
    if (pid < 0) {
        std::cerr << "Invalid PID.\n";
        return false;
    }

    DWORD dwPid = static_cast<DWORD>(pid);

    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwPid);
    if (hProcess == NULL) {
        DWORD err = GetLastError();
        std::cerr << "Failed to open process. Error code: " << err << "\n";
        return false;
    }

    if (!TerminateProcess(hProcess, 0)) {
        DWORD err = GetLastError();
        std::cerr << "Failed to terminate process. Error code: " << err << "\n";
        CloseHandle(hProcess);
        return false;
    }

    CloseHandle(hProcess);
    return true;
#else
    std::cerr << "terminateProcess is only supported on Windows.\n";
    return false;
#endif
}

 

#ifdef _WIN32
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
        return true;
    }
#endif

    bool startForegroundProcess(const std::string& command) {
        setupCtrlCHandler();
#ifdef _WIN32
        PROCESS_INFORMATION pi;
        STARTUPINFO si = { sizeof(STARTUPINFO) };
        std::string cmd = command;

        if (!CreateProcess(NULL, &cmd[0], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            std::cerr << "Failed to create process.\n";
            return false;
        }

        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
#else
        pid_t pid = fork();
        if (pid == 0) {
            execlp("/bin/sh", "sh", "-c", command.c_str(), (char *)nullptr);
            exit(1); // Không chạy được thì thoát
        } else if (pid > 0) {
            foregroundPid = pid;
            int status;
            waitpid(pid, &status, 0);
            foregroundPid = -1;
        } else {
            std::cerr << "Fork failed.\n";
            return false;
        }
#endif
        return true;
    }

    bool startBackgroundProcess(const std::string& command, bool addPause = false) {
#ifdef _WIN32
    PROCESS_INFORMATION pi;
    STARTUPINFO si = { sizeof(STARTUPINFO) };

    std::string cmdPath = command;
    if (command.find(':') == std::string::npos && command[0] != '.' && command[0] != '\\' && command[0] != '/') {
        cmdPath = ".\\" + command;
    }
    for (char& c : cmdPath) {
        if (c == '/') c = '\\'; // Chuyển đổi dấu gạch chéo sang gạch chéo ngược
    }
    // Ghép lệnh với cmd
    std::string fullCommand =  cmdPath;
    if (addPause) {
        fullCommand += " & pause";
    }

    // Dùng chuỗi mutable
    std::string mutableCmd = fullCommand;

    // Phải dùng chuỗi mutable cho CreateProcess
    std::string cmd = fullCommand; std::cout<<"Starting background process: " << cmd << "\n";

    if (!CreateProcess(NULL, &cmd[0], NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        std::cerr << "Failed to create background process.\n";
        return false;
    }

    std::cout << "Background process started with PID: " << pi.dwProcessId << "\n";

    g_processes.push_back({ (int)pi.dwProcessId, command });

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
#else
    pid_t pid = fork();
    if (pid == 0) {
        setsid();
        // Linux không cần thêm pause, vì chạy terminal khác hoặc output ghi log
        execlp("/bin/sh", "sh", "-c", command.c_str(), (char *)nullptr);
        exit(1); // Chạy thất bại
    } else if (pid < 0) {
        std::cerr << "Fork failed.\n";
        return false;
    } else {
        std::cout << "Background process started with PID: " << pid << "\n";
    }
#endif
    return true;
}

void runChild(bool is_background, std::string command) {
    if (is_background) 
        process::startBackgroundProcess("programs/child.exe " + command, true); // Giả lập tiến trình con chạy lâu dài
     else
        process::startForegroundProcess("programs/child.exe " + command); // Giả lập tiến trình con chạy lâu dài
   
}


void runCountdown(bool is_background, int seconds) {
    if(is_background) 
      process::startBackgroundProcess("programs/countdown.exe " + std::to_string(seconds), true); // Giả lập đếm ngược
    else 
      process::startForegroundProcess("programs/countdown.exe " + std::to_string(seconds)); // Giả lập đếm ngược

}
void runTicTacToe(bool is_background) {
    if (is_background) 
        process::startBackgroundProcess("programs/tictactoe.exe", true); // Giả lập trò chơi Tic Tac Toe
    else 
        process::startForegroundProcess("programs/tictactoe.exe"); // Giả lập trò chơi Tic Tac Toe
    
}
void runSpinner(bool is_background) {
    if (is_background)
        process::startBackgroundProcess("programs/spinner.exe", false); // Giả lập chương trình spinner không có pause
    else
        process::startForegroundProcess("programs/spinner.exe"); // Giả lập chương trình spinner không có pause
}
void runLogger(bool is_background) {
    if (is_background)
        process::startBackgroundProcess("programs/logger.exe", false); // Giả lập chương trình ghi log không có pause
    else
        process::startForegroundProcess("programs/logger.exe"); // Giả lập chương trình ghi log không có pause
}


}