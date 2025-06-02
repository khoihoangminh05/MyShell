#pragma once
#include <vector>
#include <string>

namespace executor {
    bool run(bool is_background, std::string command, const std::vector<std::string>& tokens);
}