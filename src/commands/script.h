// script.h
#ifndef SCRIPT_H
#define SCRIPT_H

#include <string>
#include <vector>

namespace commands {
    std::string resolvePath(const std::string& path);
    void runScript(const std::vector<std::string>& args);
    std::vector<std::string> splitInput(const std::string& input);
    void executeCommand(const std::string& command, const std::vector<std::string>& args);
}

#endif // SCRIPT_H
