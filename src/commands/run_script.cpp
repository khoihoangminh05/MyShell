#include "run_script.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <cstdlib>      // For std::getenv
#include <system_error> // For std::error_code
#include <algorithm>    // For std::transfor
#include <cctype>       // For std::tolower

// Bao gồm file command_executor.h để có định nghĩa đầy đủ của executor::run
// Bạn cầsn đảm bảo đường dẫn này đúng với vị trí của command_executor.h trong dự án của bạn
#include "shell/command_executor.h" // Giả sử command_executor.h nằm trong src/shell/

namespace fs = std::filesystem;

// Hàm splitInput (được di chuyển từ header cũ)
// Đây là hàm tiện ích chung, có thể đặt trong một namespace 'utils' hoặc 'shell' nếu muốn
// hoặc giữ nó là một hàm static trong file .cpp này nếu chỉ run_script dùng.
// Hiện tại, tôi sẽ đặt nó là static để giới hạn scope trong file này.
static std::vector<std::string> split_input(const std::string &input) {
    std::vector<std::string> tokens;
    std::string current_token;
    bool in_quote = false;

    for (char c : input) {
        if (c == '"') {
            in_quote = !in_quote;
            if (!in_quote && !current_token.empty()) { // Khi đóng quote
                tokens.push_back(current_token);
                current_token.clear();
            }
        } else if (std::isspace(c) && !in_quote) {
            if (!current_token.empty()) {
                tokens.push_back(current_token);
                current_token.clear();
            }
        } else {
            current_token += c;
        }
    }
    if (!current_token.empty()) {
        tokens.push_back(current_token);
    }
    return tokens;
}

// Hàm resolve_path đã cải tiến
std::filesystem::path commands::resolve_path(const std::string &path_str)
{
    fs::path p = path_str;
    std::error_code ec;

    // 1. Kiểm tra nếu path là absolute (tuyệt đối)
    if (p.is_absolute())
    {
        if (fs::exists(p, ec) && !ec) {
            return p;
        }
        // Thử thêm các phần mở rộng mặc định nếu absolute path không tồn tại
        const std::vector<std::string> common_extensions = {".exe", ".bat", ".cmd", ".com", ".ps1"};
        for (const auto& ext : common_extensions) {
            fs::path test_path = p;
            test_path += ext;
            if (fs::exists(test_path, ec) && !ec) {
                return test_path;
            }
        }
        return p; // Trả về original path nếu absolute path không tồn tại và không tìm thấy với ext
    }

    // 2. Kiểm tra nếu file tồn tại trong thư mục hiện tại
    fs::path current_dir_path = fs::current_path(ec);
    if (ec) {
        std::cerr << "Warning: Failed to get current directory: " << ec.message() << std::endl;
        ec.clear(); // Xóa lỗi
    } else {
        fs::path local_path = current_dir_path / p;
        if (fs::exists(local_path, ec) && !ec) {
            return fs::absolute(local_path, ec); // Trả về đường dẫn tuyệt đối
        }
        // Thử thêm các phần mở rộng mặc định
        const std::vector<std::string> common_extensions = {".exe", ".bat", ".cmd", ".com", ".ps1"};
        for (const auto& ext : common_extensions) {
            fs::path test_path = local_path;
            test_path += ext;
            if (fs::exists(test_path, ec) && !ec) {
                return fs::absolute(test_path, ec);
            }
        }
    }

    // 3. Kiểm tra trong biến môi trường PATH
    const char *pathEnv = std::getenv("PATH");
    if (pathEnv)
    {
        std::string pathEnvStr(pathEnv);
        std::string delimiter = ";"; // For Windows PATH

        size_t start = 0;
        size_t end = pathEnvStr.find(delimiter);
        while (end != std::string::npos)
        {
            fs::path dir_in_path = pathEnvStr.substr(start, end - start);
            fs::path file_in_dir = dir_in_path / p;
            
            if (fs::exists(file_in_dir, ec) && !ec) {
                return fs::absolute(file_in_dir, ec);
            }
            // Thử thêm các phần mở rộng
            const std::vector<std::string> common_extensions = {".exe", ".bat", ".cmd", ".com", ".ps1"};
            for (const auto& ext : common_extensions) {
                fs::path test_path = dir_in_path / p;
                test_path += ext;
                if (fs::exists(test_path, ec) && !ec) {
                    return fs::absolute(test_path, ec);
                }
            }

            start = end + delimiter.length();
            end = pathEnvStr.find(delimiter, start);
        }
        // Xử lý phần cuối cùng sau vòng lặp
        fs::path dir_in_path = pathEnvStr.substr(start);
        fs::path file_in_dir = dir_in_path / p;
        if (fs::exists(file_in_dir, ec) && !ec) {
            return fs::absolute(file_in_dir, ec);
        }
        const std::vector<std::string> common_extensions = {".exe", ".bat", ".cmd", ".com", ".ps1"};
        for (const auto& ext : common_extensions) {
            fs::path test_path = dir_in_path / p;
            test_path += ext;
            if (fs::exists(test_path, ec) && !ec) {
                return fs::absolute(test_path, ec);
            }
        }
    }

    // Nếu không tìm thấy, trả về đường dẫn gốc
    return p;
}
namespace commands {
    void run_script(const std::vector<std::string> &args)
    {
        if (args.size() != 1)
        {
            std::cout << "Usage: run <script_file_path>" << std::endl;
            return;
        }

        fs::path scriptPath = commands::resolve_path(args[0]); // Cải tiến: dùng resolve_path
        std::error_code ec; // Cải tiến: error_code cho filesystem operations

        if (!fs::exists(scriptPath, ec) || !fs::is_regular_file(scriptPath, ec)) // Cải tiến: kiểm tra tồn tại và là file
        {
            if (ec) { std::cerr << "Error checking script file: " << ec.message() << std::endl; }
            else { std::cout << "Script file does not exist or is not a regular file: " << scriptPath << std::endl; }
            return;
        }

        std::ifstream scriptFile(scriptPath);
        if (!scriptFile.is_open()) // Cải tiến: dùng is_open()
        {
            std::cout << "Unable to open script file: " << scriptPath << std::endl;
            return;
        }

        std::string line;
        int line_num = 0; // Cải tiến: Để theo dõi số dòng
        while (std::getline(scriptFile, line))
        {
            line_num++;
            // Cải tiến: Bỏ qua dòng trống hoặc dòng comment
            if (line.empty() || line[0] == '#') {
                continue;
            }

            auto tokens = split_input(line); // Cải tiến: Dùng hàm split_input
            bool is_background = false;

            // Check if last token is '&' → background execution
            if (!tokens.empty() && tokens.back() == "&") {
                is_background = true;
                tokens.pop_back(); // Remove '&' from tokens
            }

         
            if (!tokens.empty())
            {
                std::string command = tokens[0];
                tokens.erase(tokens.begin());
                
                std::cout << "\nExecuting (line " << line_num << "): " << line << std::endl; // Cải tiến: In lệnh đang chạy
                bool continue_shell = executor::run(is_background, command, tokens); // Cải tiến: Gọi executor::run

                if (continue_shell) { // Cải tiến: Nếu lệnh yêu cầu thoát shell, 
                    std::cout << "Script terminated by 'exit' command on line " << line_num << "." << std::endl;
                    // Có thể cần một cách để báo hiệu cho main loop thoát
                    // Tạm thời, sẽ chỉ thoát khỏi script
                    break; 
                }
            }
        }
        scriptFile.close();
        std::cout << "\nScript finished: " << scriptPath << std::endl;
    }
}