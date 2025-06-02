#include "command_executor.h"
#include "../commands/help.h"
#include "../commands/set_color.h"
#include "../utils/system_utils.h"
#include "../process/process.h"
#include <iostream>

namespace executor {
    bool run(bool is_background, std::string command, const std::vector<std::string>& tokens) {

        if (command == "help") {
            commands::help();
        } 
        else if (command == "exit") {
            return true; // Exit the shell
        } 
        else if (command == "clear") {
            sysutils::clear_screen();
        }
        else if (command == "calc") {
            sysutils::open_calculator();
        } 
        else if (command == "setcolor") {
            commands::setcolor(tokens);
        } 
        else if (command == "sys_info") {
            sysutils::print_all_system_info();
        } 
        else if (command == "date") {
            sysutils::print_current_date();
        } 
        else if (command == "time") {
            sysutils::print_current_time();
        } 
        else if (command == "cpu_info") {
            sysutils::print_cpu_info();
        } 
        else if (command == "memory_info") {
            sysutils::print_memory_info();
        } 
        else if (command == "disk_info") {
            sysutils::print_disk_info();
        } 
        else if (command == "computer_name") {
            sysutils::print_computer_name();
        } 
        else if (command == "os_info") {
            sysutils::print_os_info();
        } 
        else if (command == "terminate") {
            if (tokens.size() != 1) {
                std::cout << "Usage: terminate <pid>\n";
                return false; // Continue running the shell
            }
            int pid = std::stoi(tokens[0]);
            if (!process::terminateProcess(pid)) {
                std::cout << "Failed to terminate process with PID " << pid << ".\n";
            } else {
                std::cout << "Process with PID " << pid << " terminated successfully.\n"; 
            }
        } 
        else if (command == "suspend") {
            if (tokens.size() != 1) {
                std::cout << "Usage: suspend <pid>\n";
                return false; // Continue running the shell
            }
            int pid = std::stoi(tokens[0]);
            if (!process::suspendProcess(pid)) {
                std::cout << "Failed to suspend process with PID " << pid << ".\n";
            } else {
                std::cout << "Process with PID " << pid << " suspended successfully.\n";
            }
        } 
        else if (command == "resume") {
            if (tokens.size() != 1) {
                std::cout << "Usage: resume <pid>\n";
                return false; // Continue running the shell
            }
            int pid = std::stoi(tokens[0]);
            if (!process::resumeProcess(pid)) {
                std::cout << "Failed to resume process with PID " << pid << ".\n";
            } else {
                std::cout << "Process with PID " << pid << " resumed successfully.\n";
            }    
        }
        else if (command == "list") {
            process::listProcesses();
        } 
        else if(command == "child") {
            // Chạy tiến trình con để kiểm tra chức năng
            process::runChild(is_background, tokens.empty() ? "" : tokens[0]);
        }
        else if(command == "countdown") {
            // Chạy chương trình đếm ngược
            int seconds = 10; // Mặc định là 10 giây
            if (!tokens.empty()) {
                try {
                    seconds = std::stoi(tokens[0]);
                } catch (const std::exception& e) {
                    std::cout << "Invalid argument for countdown: " << tokens[0] << "\n";
                    return false; // Continue running the shell
                }
            }
            process::runCountdown(is_background, seconds);
        } 
        else if (command == "tictactoe") {
            // Chạy trò chơi Tic Tac Toe
            process::runTicTacToe(is_background);
        } 
        else if (command == "spinner") {
            // Chạy chương trình spinner
            process::runSpinner(is_background);
        } 
        else if (command == "logger") {
            // Chạy chương trình ghi log
            process::runLogger(is_background);
        } 
        
        else if (is_background) {
            // Xử lý lệnh đặc biệt để chạy tiến trình nền
            if (tokens.empty()) {
                std::cout << "No command provided for background execution.\n";
                return false; // Continue running the shell
            }
            command = tokens[0]; // Lấy lệnh đầu tiên
            std::vector<std::string> args(tokens.begin() + 1, tokens.end()); // Các tham số còn lại

            // Chuyển đổi thành chuỗi lệnh
            for (const auto& arg : args) {
                command += " " + arg;
            }
        
        process::startBackgroundProcess(command, true); // Thêm dấu nhắc tạm dừng
            return false; // Continue running the shell
    } else {
            // Xử lý lệnh bình thường
            if (tokens.empty()) {
                std::cout << "No command provided.\n";
                return false; // Continue running the shell
            }
            //command = command + " " + tokens[0]; // Lấy lệnh đầu tiên
            //std::vector<std::string> args(tokens.begin() + 1, tokens.end()); // Các tham số còn lại

            // Chuyển đổi thành chuỗi lệnh
            for (const auto& arg : tokens) {
                command += " " + arg;
            }
        process::startForegroundProcess(command);
    }
        //else {
       //     std::cout << "Unknown command: " << command << "\n";
       // }
       return false; // Continue running the shell
    }
}