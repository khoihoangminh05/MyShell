#include "environment.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <windows.h>

namespace environment{

    // Khởi tạo danh sách các lệnh được hỗ trợ
    const std::unordered_set<std::string> supportedCommands = {
        "add_path",
        "remove_path",
        "print_env",
        "set_env",
        "unset_env",
        "is_in_path",
        "load_env"
    };

    // Trả về danh sách các lệnh hỗ trợ
    const std::unordered_set<std::string> &getSupportedCommands() {
        return supportedCommands;
    }

    // Thêm đường dẫn vào PATH
    void addToPath(const std::string &path) {
        std::string currentPath = getEnv("PATH");
        if (currentPath.find(path) == std::string::npos) {
            std::string newPath = currentPath + ";" + path;
            setEnv("PATH", newPath);
            std::cout << "Added " << path << " to PATH.\n";
        } else {
            std::cout << path << " is already in PATH.\n";
        }
    }

    // Xóa đường dẫn khỏi PATH
    void removeFromPath(const std::string &path) {
        std::string currentPath = getEnv("PATH");
        size_t pos = currentPath.find(path);
        if (pos != std::string::npos) {
            std::string newPath;
            if (pos > 0 && currentPath[pos - 1] == ';') {
                newPath = currentPath.substr(0, pos - 1) + currentPath.substr(pos + path.length());
            } else if (pos + path.length() < currentPath.length() && currentPath[pos + path.length()] == ';') {
                newPath = currentPath.substr(0, pos) + currentPath.substr(pos + path.length() + 1);
            } else {
                newPath = currentPath.substr(0, pos) + currentPath.substr(pos + path.length());
            }
            setEnv("PATH", newPath);
            std::cout << "Removed " << path << " from PATH.\n";
        } else {
            std::cout << path << " is not in PATH.\n";
        }
    }

    // In ra giá trị của biến môi trường
    void printEnv(const std::string &var) {
        std::string value = getEnv(var);
        if (!value.empty()) {
            std::cout << var << " = " << value << '\n';
        } else {
            std::cout << var << " is not set.\n";
        }
    }

    // Thiết lập giá trị biến môi trường
    void setEnv(const std::string &var, const std::string &value) {
        std::string actualValue = getEnv(value);
        if (!actualValue.empty()) {
            _putenv_s(var.c_str(), actualValue.c_str());
        } else {
            _putenv_s(var.c_str(), value.c_str());
        }
        std::cout<<"set variable "<< var <<" at value of "<<value<<"\n";
    }

    // Xóa biến môi trường
    void unsetEnv(const std::string &var) {
        _putenv_s(var.c_str(), "");
        std::cout << "Unset variable " << var << '\n';
    }

    // Kiểm tra sự hiện diện của đường dẫn trong PATH
    bool isInPath(const std::string &path) {
        std::string currentPath = getEnv("PATH");
        return currentPath.find(path) != std::string::npos;
    }


    //  // Hàm để liệt kê tất cả các biến môi trường
    // void listAllEnv()
    // {
    //     extern char **environ;
    //     for (char **env = environ; *env; ++env)
    //     {
    //         std::cout << *env << std::endl;
    //     }
    // }

    // // Hàm để lưu biến môi trường vào file
    // void saveEnvToFile(const std::string &filename)
    // {
    //     std::ofstream file(filename);
    //     if (!file)
    //     {
    //         std::cerr << "Could not open file for writing: " << filename << std::endl;
    //         return;
    //     }
    //     extern char **environ;
    //     for (char **env = environ; *env; ++env)
    //     {
    //         file << *env << std::endl;
    //     }
    //     file.close();
    //     std::cout << "Environment variables saved to " << filename << std::endl;
    // }

    // Tải biến môi trường từ file
    void loadEnvFromFile(const std::string &filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Could not open file for reading: " << filename << '\n';
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string var = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                setEnv(var, value);
            }
        }
        file.close();
        std::cout << "Environment variables loaded from " << filename << '\n';
    }

    // Lấy giá trị biến môi trường
    std::string getEnv(const std::string &var) {
        const char *value = getenv(var.c_str());
        if (value) {
            return std::string(value);
        }
        return "";
    }
}