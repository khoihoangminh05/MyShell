#include "input_parser.h"
#include <sstream>

namespace parser {
    std::vector<std::string> tokenize(const std::string& input) {
        std::istringstream iss(input);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }
}