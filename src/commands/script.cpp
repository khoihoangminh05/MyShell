// script.cpp
#include "script.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <cstdlib>
#include "../shell/input_parser.h"
#include "../shell/command_executor.h"

namespace fs = std::filesystem;

namespace commands {


    std::vector<std::string> splitInput(const std::string& input) {
        std::vector<std::string> tokens;
        std::string token;
        for (char c : input) {
            if (isspace(c)) {
                if (!token.empty()) {
                    tokens.push_back(token);
                    token.clear();
                }
            } else {
                token += c;
            }
        }
        if (!token.empty()) {
            tokens.push_back(token);
        }
        return tokens;
    }

    std::string resolvePath(const std::string& path) {
        if (fs::path(path).is_absolute()) {
            return path;
        }

        if (fs::exists(path)) {
            return fs::absolute(path).string();
        }

        const char* pathEnv = std::getenv("PATH");
        if (pathEnv) {
            std::vector<std::string> paths;
            std::string pathEnvStr(pathEnv);
            std::string delimiter = ";";

            size_t pos = 0;
            std::string token;
            while ((pos = pathEnvStr.find(delimiter)) != std::string::npos) {
                token = pathEnvStr.substr(0, pos);
                paths.push_back(token);
                pathEnvStr.erase(0, pos + delimiter.length());
            }
            paths.push_back(pathEnvStr);

            for (const auto& p : paths) {
                fs::path filePath = fs::path(p) / path;
                if (fs::exists(filePath)) {
                    return fs::absolute(filePath).string();
                }
            }
        }

        return path;
    }

    void runScript(const std::vector<std::string>& args) {
        if (args.size() != 1) {
            std::cerr << "Usage: run <script_file_path>" << std::endl;
            return;
        }

        std::string scriptPath = resolvePath(args[0]);
        std::ifstream scriptFile(scriptPath);
        if (!scriptFile) {
            std::cerr << "Unable to open script file: " << scriptPath << std::endl;
            return;
        }

        std::string line;
        while (std::getline(scriptFile, line)) {
            auto tokens = parser::tokenize(line);
            if (tokens.empty()) {
                continue; // Skip empty lines
            }
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
        scriptFile.close();
    }

} // namespace commands
