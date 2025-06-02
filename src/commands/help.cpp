#include "help.h"
#include <iostream>

namespace commands {
    void help() {
    std::cout << "myShell - A simple Windows shell\n\n";
    std::cout << "Built-in commands:\n";
    std::cout << "  help             Show this help message\n";
    std::cout << "  cd <dir>         Change current directory\n";
    std::cout << "  exit             Exit the shell\n";
    std::cout << "  time             Show current time\n";
    std::cout << "  date             Show current date\n";
    std::cout << "  clear            Clear the screen\n";
    std::cout << "  calc             Open a simple calculator\n";
    std::cout << "  sysinfo          Show system information\n";
    std::cout << "  addpath <path>   Add path to PATH\n";
    std::cout << "  dir              List current directory contents\n";
    std::cout << "\nShell usage:\n";
    std::cout << "  - Use normal Windows commands like `ping`, `notepad`, etc.\n";
    std::cout << "  - Prefix command with `$` to run in background (e.g., $ ping google.com)\n";
    std::cout << "  - Press Ctrl+C to stop a foreground process\n";
    std::cout << R"(
Available commands:
- run <command>        : Run a background process
- list                 : List tracked processes
- terminate <pid>      : Terminate a process
- suspend <pid>        : Suspend a process (Windows only)
- resume <pid>         : Resume a process (Windows only)
- help                 : Show this help
- exit                 : Exit program
)";
    }
}