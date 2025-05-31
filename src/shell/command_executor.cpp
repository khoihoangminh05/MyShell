#include "command_executor.h"
#include "../commands/help.h"
#include "../commands/set_color.h"
#include "../utils/system_utils.h"
#include <iostream>

namespace executor {
    bool run(std::string command, const std::vector<std::string>& tokens) {

        if (command == "help") {
            commands::help();
        } 
        else if (command == "exit") {
            return true; // Exit the shell
        } 
        else if (command == "set_color") {
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
        else {
            std::cout << "Unknown command: " << command << "\n";
        }
        return false; // Continue running the shell
    }
}