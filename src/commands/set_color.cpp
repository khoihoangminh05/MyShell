#include "set_color.h"
#include <windows.h>
#include <iostream>
#include <vector>

namespace commands {
    void setcolor(const std::vector<std::string>& tokens) {
        if (tokens.empty()) {
            std::cout << "Usage: set_color <color_code>\n";
            std::cout << "Available color codes: 0-15\n";
            return;
        }

        try {
            int colorCode = std::stoi(tokens[0]);
            if (colorCode < 0 || colorCode > 15) {
                std::cout << "Color code must be between 0 and 15.\n";
                return;
            }
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorCode);
            std::cout << "Text color changed to " << colorCode << ".\n";
        } catch (const std::invalid_argument&) {
            std::cout << "Invalid color code. Please enter a number between 0 and 15.\n";
        } catch (const std::out_of_range&) {
            std::cout << "Color code out of range. Please enter a number between 0 and 15.\n";
        }
    }
}