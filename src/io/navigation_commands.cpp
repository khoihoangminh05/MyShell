#include "navigation_commands.h"
#include <iostream>
#include <filesystem>
#include <system_error> // Cho std::error_code

namespace fs = std::filesystem;
namespace io{
    void change_directory(const std::vector<std::string> &args)
    {
        if (args.size() != 1)
        {
            std::cout << "Usage: cd <directory_path>" << std::endl;
            return;
        }
        fs::path dirPath = args[0];
        std::error_code ec; // Cải tiến: Sử dụng error_code để tránh exception

        if (!fs::exists(dirPath, ec)) // Cải tiến: Kiểm tra tồn tại với error_code
        {
            if (ec) { std::cerr << "Error checking path existence: " << ec.message() << std::endl; }
            else { std::cout << "Directory does not exist: " << dirPath << std::endl; }
            return;
        }
        if (!fs::is_directory(dirPath, ec)) // Cải tiến: Kiểm tra là thư mục với error_code
        {
            if (ec) { std::cerr << "Error checking path type: " << ec.message() << std::endl; }
            else { std::cout << "Path is not a directory: " << dirPath << std::endl; }
            return;
        }

        fs::current_path(dirPath, ec); // Cải tiến: Thay đổi path với error_code
        if (ec) {
            std::cerr << "Failed to change directory to '" << dirPath.string() << "': " << ec.message() << std::endl;
        } else {
            std::cout << "Changed directory to: " << fs::current_path().string() << std::endl;
        }
    }

    void list_directory_contents(const std::vector<std::string> &args)
    {
        fs::path dirPath = "."; // Mặc định là thư mục hiện tại
        if (args.size() == 1)
        {
            dirPath = args[0];
        }
        else if (args.size() > 1) { // Cải tiến: Xử lý khi có quá nhiều đối số
            std::cout << "Usage: ls [directory_path]" << std::endl;
            return;
        }

        std::error_code ec;
        
        // Cải tiến: Kiểm tra tồn tại và là thư mục với error_code
        if (!fs::exists(dirPath, ec)) {
            if (ec) { std::cerr << "Error checking path existence: " << ec.message() << std::endl; }
            else { std::cout << "Directory does not exist: " << dirPath << std::endl; }
            return;
        }
        if (!fs::is_directory(dirPath, ec)) {
            if (ec) { std::cerr << "Error checking path type: " << ec.message() << std::endl; }
            else { std::cout << "Path is not a directory: " << dirPath << std::endl; }
            return;
        }

        std::cout << "Contents of '" << dirPath.string() << "':\n";
        try { // Cải tiến: try-catch cho directory_iterator để bắt lỗi nghiêm trọng
            for (const auto &entry : fs::directory_iterator(dirPath, ec)) // Cải tiến: Dùng iterator với error_code
            {
                if (ec) { // Cải tiến: Kiểm tra lỗi sau mỗi lần lấy entry
                    std::cerr << "Warning: Error iterating directory '" << dirPath.string() << "': " << ec.message() << std::endl;
                    ec.clear(); // Xóa lỗi để không ảnh hưởng lần lặp tiếp theo
                    continue; // Bỏ qua entry bị lỗi
                }
                std::cout << entry.path().filename().string();
                // Cải tiến: is_directory dùng entry.status() để tránh gọi lại filesystem
                if (fs::is_directory(entry.status(ec))) { 
                    std::cout << "/"; // Thêm dấu gạch chéo cho thư mục
                } else if (fs::is_symlink(entry.status(ec))) { // Cải tiến: Hiển thị ký hiệu cho symlink
                    std::cout << "@";
                }
                std::cout << std::endl;
                if (ec) ec.clear(); // Cải tiến: Xóa lỗi sau khi kiểm tra status
            }
        } catch (const fs::filesystem_error& e) { // Cải tiến: Bắt lỗi nếu iterator ném exception
            std::cerr << "Filesystem error listing directory: " << e.what() << std::endl;
        }
    }

    void print_working_directory(const std::vector<std::string> &args)
    {
        if (!args.empty()) { // Cải tiến: Kiểm tra không có đối số
            std::cout << "Usage: pwd (no arguments)\n";
            return;
        }
        std::error_code ec; // Cải tiến: Sử dụng error_code
        fs::path currentPath = fs::current_path(ec);
        if (ec) {
            std::cerr << "Failed to get current directory: " << ec.message() << std::endl;
        } else {
            std::cout << "Current Directory: " << currentPath.string() << std::endl;
        }
    }
}