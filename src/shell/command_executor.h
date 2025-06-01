#pragma once
#include <vector>
#include <string>

namespace executor {
    bool run(const std::string& command, const std::vector<std::string>& tokens);
}