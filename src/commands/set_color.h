#pragma once
#include <string>
#include <vector>

namespace commands {
    int getColorCode(const std::string& colorName);
    void setcolor(const std::vector<std::string>& tokens);
}