#include "shell.h"
#include "command_executor.h"
#include "input_parser.h"
#include <iostream>

namespace shell {
    void start() {
        std::cout << "\n===== Welcome to Tiny Shell! =====\n";
        std::string input;
        while (true) {
            std::cout << ">> ";
            std::getline(std::cin, input);
            auto tokens = parser::tokenize(input);

            std::string command = tokens.empty() ? "" : tokens[0];
            tokens.erase(tokens.begin()); // Remove command from tokens

            if(!executor::run(command, tokens)) {
                break;
            }
        }

        std::cout << "Exiting Tiny Shell. Goodbye!\n";
    }
}