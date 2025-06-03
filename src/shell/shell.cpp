#include "shell.h"
#include "command_executor.h"
#include "input_parser.h"
#include <iostream>
#include <windows.h>

namespace shell {

    void setTextColor(int color) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }

    void printBanner() {
        setTextColor(11); // Light cyan
        std::cout << R"(
 __  __       ____  _          _ _ 
|  \/  |_   _/ ___|| |__   ___| | |
| |\/| | | | \___ \| '_ \ / _ \ | |
| |  | | |_| |___) | | | |  __/ | |
|_|  |_|\__, |____/|_| |_|\___|_|_|
        |___/        MyShell
)";
        setTextColor(7); // Reset to white
        std::cout << "\nWelcome to MyShell! Type 'help' to see available commands.\n\n";
    }

    void start() {
        printBanner();
        std::string input;
        std::string cur_color;
        while (true) {

             CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
            int currentColor = csbi.wAttributes;

        
            setTextColor(10);
            std::cout << "MyShell";

            setTextColor(currentColor);
            std::cout << ">> ";

            std::getline(std::cin, input);
       
            auto tokens = parser::tokenize(input);
            bool is_background = false;

            // Check if last token is '&' → background execution
            if (!tokens.empty() && tokens.back() == "&") {
                is_background = true;
                tokens.pop_back(); // Remove '&' from tokens
            }

            std::string command = tokens.empty() ? "" : tokens[0];
            tokens.erase(tokens.begin()); // Remove command from tokens

            if (executor::run(is_background, command, tokens)) {
                break;
            }
        }

        setTextColor(14); // Yellow
        std::cout << "Exiting MyShell. Goodbye!\n";
        setTextColor(7); // Reset
    }
}
