#include "command_executor.h"
#include "../commands/help.h"
#include "../commands/set_color.h"
#include "../commands/script.h"
#include "../utils/system_utils.h"
#include "../process/process.h"
#include "../io/directory_commands.h"
#include "../io/navigation_commands.h"
#include "../io/file_commands.h" 
#include "../core/environment.h"
#include "../core/scheduler.h"
#include <iostream>

namespace executor {
    bool run(bool is_background, std::string command, const std::vector<std::string>& tokens) {

        // --- Lệnh chung ---
        if (command == "help") {
            commands::help();
        } 
        else if (command == "exit") {
            return true; // Dừng vòng lặp shell
        } 
        else if (command == "clear") {
            sysutils::clear_screen();
        }
        else if (command == "calc") {
            sysutils::open_calculator();
        } 
        else if (command == "run") {
            //std::cout<<"Running script: " << tokens[0] << std::endl;
            commands::runScript(tokens);
        }
         // --- Lệnh tiện ích shell ---
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
        // --- Lệnh quản lý thư mục ---
        else if (command == "copy") {
            io::copy_directory(tokens);
        }
        else if (command == "create") {
            io::create_directory(tokens);
        }
        else if (command == "delete") {
            io::delete_directory(tokens);
        }
        else if (command == "list_tree") {
            io::list_directory_tree(tokens);
        }
        else if (command == "move") {
            io::move_directory(tokens);
        }
        else if (command == "rename") {
            io::rename_directory(tokens);
        }
        else if (command == "is_empty") {
            io::is_directory_empty(tokens);
        }
        else if (command == "get_size") {
            io::get_directory_size(tokens);
        }
        // --- Lệnh quản lý file và thông tin đường dẫn (từ file_commands.cpp) --- // NEW
        else if (command == "touch") {
            io::create_file_command(tokens); // Đổi tên hàm
        }
        else if (command == "rm") {
            io::delete_file_command(tokens); // Đổi tên hàm
        }
        else if (command == "rename_file") {
            io::rename_file(tokens);
        }
        else if (command == "copy_file") {
            io::copy_file_command(tokens); // NEW
        }
        else if (command == "move_file") {
            io::move_file_command(tokens); // NEW
        }
        else if (command == "write_file") {
            io::write_file(tokens); // NEW
        } 
        else if (command == "read_file") {
            io::read_file(tokens); // NEW
        }
        else if (command == "delete_file") {
            io::delete_file_command(tokens); // Đổi tên hàm
        }
        else if (command == "list_ext") {
            io::list_files_with_extension(tokens); // NEW
        }
        else if (command == "extensions") {
            io::print_file_extensions(tokens); // NEW
        }
        else if (command == "file_info") {
            io::show_file_info(tokens); // NEW
        }
        else if (command == "open") {
            io::open_path(tokens); // NEW
        }
        else if (command == "exists") {
            io::check_file_existence(tokens); // NEW
        }
        else if (command == "abspath") {
            io::get_absolute_path_command(tokens); // NEW
        }
        else if (command == "normalize") {
            io::get_normalized_path_command(tokens); // NEW
        }
        else if (command == "compare") {
            io::compare_files(tokens); // NEW
        }
        // --- Lệnh điều hướng (từ navigation_commands.cpp) ---
        else if (command == "pwd") {
            io::print_working_directory(tokens);
        }
        else if (command == "cd") {
            io::change_directory(tokens);
        }
        else if (command == "ls") {
            io::list_directory_contents(tokens);
        }
        // --- Lệnh quản lý môi trường ---
        else if (command == "add_path") {
            // Thêm đường dẫn vào PATH
            if (tokens.size() != 1) {
                std::cout << "Usage: add_path <directory_path>\n";
            } else {
                environment::addToPath(tokens[0]);
            }
        } else if (command == "remove_path"){
            // Xoá đường dẫn khỏi PATH
            if (tokens.size() != 1) {
                std::cout << "Usage: add_path <directory_path>\n";
            } else {
                environment::removeFromPath(tokens[0]);
            }
        } else if (command == "print_env") {
            // In ra giá trị biến môi trường
             if (tokens.size() != 1) {
                std::cout << "No variable provided";
            } else {
                environment::printEnv(tokens[0]);
            }
        } else if (command == "set_env") {
            if (tokens.size() != 2) {
                std::cout << "Usage: Enter the variable name and value\n";
            } else {
                environment::setEnv(tokens[1], tokens[0]);
            }
        } else if (command == "unset_env") {
             if (tokens.size() != 1) {
                std::cout << "Usage: Enter the variable name\n";
            } else {
                environment::unsetEnv(tokens[0]);
            }
        } else if (command == "is_in_path") {
            if (tokens.size() != 1) {
                std::cout << "Usage: Enter the variable name\n";
            } else {
                std::cout << environment::isInPath(tokens[0]) << "\n";
            }
        } 
        // else if (command == "list_env") {
        //     environment::listAllEnv();
        // } else if (command == "save_env") {
        //     if (tokens.size() != 1) {
        //         std::cout<<"Usage: Enter file name\n";
        //     } else {
        //         environment::saveEnvToFile(tokens[0]);
        //     }
        // } else if (command == "load_env") {
        //     if (tokens.size() != 1) {
        //         std::cout<<"Usage: Enter file name\n";
        //     } else {
        //         environment::loadEnvFromFile(tokens[0]);
        //     }
        // } 
        else if (command == "get_env") {
            if (tokens.size() != 1) {
                std::cout<<"Usage: Enter variable name\n";
            } else {
                environment::getEnv(tokens[0]);
            }
        } else if (command == "delay_command") {
           return scheduler::scheduleCommand(tokens);
        }
        // ---- Lệnh quản lý tiến trình ---- 
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
        else if (command == "list_proc") {
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
        else if (command == "heart") {
            // Chạy chương trình vẽ trái tim
            process::runHeart(is_background);
        } 
        else if (command == "duck") {
            // Chạy chương trình vẽ con vịt
            process::runDuck(is_background);
        }
        else if (command == "start") {
          if (tokens.empty()) {
              std::cout << "Usage: start <command> [&]\n";
              return false;
          }

        // Tạo lại lệnh từ tokens
            std::string cmd_line = tokens[0];

            for (size_t i = 1; i < tokens.size(); ++i) {
                cmd_line += " " + tokens[i];
            }

            if (is_background) {
                process::startBackgroundProcess(cmd_line, true);
            } else {
              process::startForegroundProcess(cmd_line);
            }

            return false; // tiếp tục shell
        }
        
        else {

        std::string full_command = command;

        for(const auto& token : tokens) {
            full_command += " " + token;
        }

        bool started = false;
        if (is_background) {
            started = process::startBackgroundProcess(full_command,true);
        } else {
            started = process::startForegroundProcess(full_command);
        }

        if (!started) {
            std::cout << "Unknown command or failed to start process: " << command << "\n";
        }
    }
       return false; // Continue running the shell
    }
}