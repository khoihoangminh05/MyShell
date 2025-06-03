#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>
#include <unordered_set>

namespace environment
{
    // Các lệnh hỗ trợ
    extern const std::unordered_set<std::string> supportedCommands;

    // Hàm lấy các lệnh hỗ trợ
    const std::unordered_set<std::string> &getSupportedCommands();

    void addToPath(const std::string &path); // Thêm đường dẫn vào biến môi trường PATH
    void removeFromPath(const std::string &path); // Xóa đường dẫn khỏi biến môi trường PATH
    void printEnv(const std::string &var); // In ra giá trị biến môi trường
    void setEnv(const std::string &var, const std::string &value); // Thiết lập giá trị biến môi trường
    void unsetEnv(const std::string &var); // Loại bỏ biến môi trường
    bool isInPath(const std::string &path); // Kiểm tra sự hiện diện của đường dẫn trong PATH
    // void listAllEnv(); // Liệt kê tất cả biến môi trường
    // void saveEnvToFile(const std::string &filename); // Lưu biến môi trường vào file
    void loadEnvFromFile(const std::string &filename); // Tải biến môi trường từ file
    std::string getEnv(const std::string &var); // Lấy giá trị biến môi trường
};

#endif // ENVIRONMENT_H