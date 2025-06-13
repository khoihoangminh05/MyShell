#include "input_parser.h"
#include <sstream>
#include <vector>
#include <string>

namespace parser {
    std::vector<std::string> tokenize(const std::string& input) {
        std::vector<std::string> tokens;
        std::istringstream iss(input);
        std::string token;
        bool inQuotes = false;
        std::string quotedToken;

        char c;
        while (iss.get(c)) {
            if (c == '"') {
                inQuotes = !inQuotes;
                if (!inQuotes) {
                    // Kết thúc chuỗi trong dấu nháy
                    tokens.push_back(quotedToken);
                    quotedToken.clear();
                }
                continue;
            }

            if (inQuotes) {
                quotedToken += c;
            } else {
                if (std::isspace(c)) {
                    if (!token.empty()) {
                        tokens.push_back(token);
                        token.clear();
                    }
                } else {
                    token += c;
                }
            }
        }

        // Đẩy token cuối nếu còn
        if (!token.empty()) {
            tokens.push_back(token);
        }

        return tokens;
    }
}
