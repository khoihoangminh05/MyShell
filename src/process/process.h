#pragma once

#include <string>

namespace process {

    // Hàm tạo tiến trình foreground
    bool startForegroundProcess(const std::string& command);

    // Hàm tạo tiến trình background
    bool startBackgroundProcess(const std::string& command, bool addPause);

    // Hàm khởi tạo Ctrl+C handler
    void setupCtrlCHandler();

    void signalHandler(int signal);

    void listProcesses();
    bool terminateProcess(int pid);

    void runChild(bool is_background, std::string command);

    void runCountdown(bool is_background, int seconds);

    void runTicTacToe(bool is_background);
    void runSpinner(bool is_background);
    void runLogger(bool is_background);

#ifdef _WIN32
    bool suspendProcess(int pid);
    bool resumeProcess(int pid);
#endif

} // namespace process

