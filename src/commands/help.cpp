#include "help.h"
#include <iostream>

namespace commands {
    void help() {
        std::cout << "\nAvailable commands:\n";
        std::cout << "  help        Show this help\n";
        std::cout << "  exit        Exit the shell\n";
        std::cout << "  set_color    Change text color (Windows only)\n";
    }
}