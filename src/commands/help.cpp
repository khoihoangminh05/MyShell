#include "help.h"
#include <iostream>
#include <map>
#include <iomanip>
#include <vector>

namespace commands {

    const std::map<std::string, std::vector<std::pair<std::string, std::string>>> command_groups = {
        {"General Commands", {
            {"help",        "Show this help message"},
            {"exit",        "Exit the shell"},
            {"run",         "Execute commands from a script file: run <script_file_path>"}
        }},
        {"Directory Management (Fs commands)", {
            {"create",      "Create a new directory: create <directory_path>"},
            {"copy",        "Copy a directory: copy <source_path> <destination_path>"},
            {"delete",      "Delete a directory (recursive): delete <directory_path>"},
            {"move",        "Move/rename a directory: move <source_path> <destination_path>"},
            {"rename",      "Rename a directory: rename <old_path> <new_path>"},
            {"list_tree",   "List directory tree: list_tree <directory_path>"},
            {"is_empty",    "Check if a directory is empty: is_empty <directory_path>"},
            {"get_size",    "Calculate directory size (bytes): get_size <directory_path>"}
        }},
        {"File Management (File commands)", { // RENAME TO THIS GROUP
            {"touch",       "Create an empty file: touch <file_path>"},
            {"rm",          "Delete a file: rm <file_path>"},
            {"rename_file", "Rename a file: rename_file <old_path> <new_path>"},
            {"copy_file",   "Copy a file: copy_file <source> <destination>"}, // NEW
            {"move_file",   "Move a file: move_file <source> <destination>"}, // NEW
            {"list_ext",    "List files with extension: list_ext [directory] <extension>"}, // NEW
            {"extensions",  "Print file extensions: extensions <file1> [...]"}, // NEW
            {"file_info",   "Show file size and line count: file_info <filename>"}, // NEW
            {"open",        "Open file/folder with default app: open <path>"}, // NEW
            {"exists",      "Check if a path exists: exists <path>"},
            {"abspath",     "Get absolute path of a path: abspath <path>"},
            {"normalize",   "Normalize a path: normalize <path>"},
            {"attrib",      "Get file/directory attributes: attrib <path>"},
            {"set_readonly","Set file to read-only: set_readonly <file_path>"},
            {"set_writeable","Set file to write-enable: set_writeable <file_path>"},
            {"compare",     "Compare two files: compare <file1_path> <file2_path>"}
        }},
        {"Navigation Commands (Nav commands)", { // KEEP THIS GROUP
            {"pwd",         "Print current working directory"},
            {"cd",          "Change current directory: cd <directory_path>"},
            {"ls",          "List directory contents: ls [directory_path]"}
        }},
        {"System Information (Sys commands)", { // KEEP THIS GROUP
            {"sys_info",    "Print all general system information"},
            {"os_info",     "Print operating system information"},
            {"cpu_info",    "Print CPU information"},
            {"memory_info", "Print memory information"},
            {"disk_info",   "Print disk drive information"},
            {"computer_name", "Print computer name"},
            {"date",        "Print current date"},
            {"time",        "Print current time"}
        }},
        {"Shell Utilities", {
            {"set_color",   "Change text color (Windows only): set_color <color_code> (0-15)"}
        }}
    };


    void help() {
        std::cout << "\n--- Available commands ---\n";
        for (const auto& group : command_groups) {
            std::cout << "\n" << group.first << ":\n";
            for (const auto& cmd_pair : group.second) {
                std::cout << "  " << std::left << std::setw(15) << cmd_pair.first << cmd_pair.second << "\n";
            }
        }
        std::cout << "--------------------------\n";
    }
} // namespace commands