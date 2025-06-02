#include "set_color.h"
#include <windows.h>
#include <iostream>
#include <unordered_map>
#include <algorithm>

namespace commands {

    int getColorCode(const std::string& colorName) {
        static const std::unordered_map<std::string, int> colorMap = {
            {"black", 0},
            {"blue", 1},
            {"green", 2},
            {"aqua", 3},        // cyan
            {"red", 4},
            {"purple", 5},      // magenta
            {"yellow", 6},
            {"white", 7},
            {"gray", 8},
            {"lightblue", 9},
            {"lightgreen", 10},
            {"lightaqua", 11},
            {"lightred", 12},
            {"lightpurple", 13},
            {"lightyellow", 14},
            {"brightwhite", 15}
        };

        auto it = colorMap.find(colorName);
        if (it != colorMap.end()) {
            return it->second;
        }
        return -1; // not found
    }

    void setcolor(const std::vector<std::string>& tokens) {
        if (tokens.empty()) {
            std::cout << "Usage: setcolor <color_name>\n";
            std::cout << "Available colors: black, blue, green, aqua, red, purple, yellow, white, gray, lightblue, lightgreen, lightaqua, lightred, lightpurple, lightyellow, brightwhite\n";
            return;
        }

        std::string colorName = tokens[0];
        // Convert to lowercase for case-insensitivity
        std::transform(colorName.begin(), colorName.end(), colorName.begin(), ::tolower);

        int colorCode = getColorCode(colorName);
        if (colorCode == -1) {
            std::cout << "Invalid color name. Type 'setcolor' to see the list of available colors.\n";
            return;
        }

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorCode);
        std::cout << "Text color changed to '" << colorName << "' (" << colorCode << ").\n";
    }
}
