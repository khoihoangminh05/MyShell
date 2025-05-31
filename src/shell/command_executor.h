#pragma once
#include <vector>
#include <string>

namespace executor {
    bool run(std::string command, const std::vector<std::string>& tokens);
}