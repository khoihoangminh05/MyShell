#include "command_executor.h"
#include "../commands/help.h"
#include "../commands/set_color.h"
#include "../io/directory_commands.h"
#include "../io/navigation_commands.h"
#include "../io/file_commands.h"      // NEW: Include file commands
#include "../utils/system_utils.h"
#include "../commands/run_script.h"
#include <iostream>
#include <algorithm>
#include <cctype>

namespace executor {
    bool run(const std::string& command_str, const std::vector<std::string>& tokens) {
        std::string command = command_str;
        std::transform(command.begin(), command.end(), command.begin(), ::tolower);

        if (command.empty()) {
            return true;
        }
        
        // --- Lệnh chung ---
        if (command == "help") {
            commands::help();
        } 
        else if (command == "exit") {
            return false;
        }
        else if (command == "run") {
            commands::run_script(tokens);
        }
        // --- Lệnh tiện ích shell ---
        else if (command == "set_color") {
            commands::setcolor(tokens);
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
        // --- Lệnh tiện ích hệ thống (giữ nguyên trong sysutils) ---
        else if (command == "sys_info") {
            sysutils::print_all_system_info();
        } 
        else if (command == "os_info") {
            sysutils::print_os_info();
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
        else if (command == "date") {
            sysutils::print_current_date();
        } 
        else if (command == "time") {
            sysutils::print_current_time();
        } 
        else {
            std::cout << "Unknown command: " << command_str << "\n";
        }
        return true;
    }
} // namespace executor