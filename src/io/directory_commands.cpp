#include "directory_commands.h" // Include header của chính nó
#include <iostream>
#include <filesystem>
#include <system_error> // Cho std::error_code
#include <limits>       // Cho std::numeric_limits
#include <algorithm>    // Cho std::transform
#include <iomanip>      // Cho std::quoted

namespace fs = std::filesystem; // Alias cho std::filesystem

// Khởi tạo bộ sưu tập các lệnh quản lý thư mục
const std::unordered_set<std::string> directory_commands = {
    "copy",
    "create",
    "delete",
    "list_tree",
    "move",
    "rename",
    "is_empty",
    "get_size"
};

// Hàm trợ giúp để xóa bộ đệm input
// Đây là hàm nội bộ, có thể đặt static để giới hạn scope trong file này
namespace io{
    static void clear_input_buffer() {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // --- Các hàm quản lý thư mục đã cải tiến ---

    void copy_directory(const std::vector<std::string> &args)
    {
        if (args.size() != 2)
        {
            std::cout << "Usage: copy <source_path> <destination_path>" << std::endl;
            return;
        }
        fs::path sourcePath = args[0];
        fs::path destinationPath = args[1];
        std::error_code ec;

        if (!fs::exists(sourcePath, ec))
        {
            if (ec) { std::cerr << "Error checking source path existence: " << ec.message() << std::endl; }
            else { std::cout << "Source path does not exist: " << sourcePath << std::endl; }
            return;
        }
        if (!fs::is_directory(sourcePath, ec))
        {
            if (ec) { std::cerr << "Error checking source path type: " << ec.message() << std::endl; }
            else { std::cout << "Source path is not a directory: " << sourcePath << std::endl; }
            return;
        }
        if (fs::exists(destinationPath, ec))
        {
            if (ec) { std::cerr << "Error checking destination path existence: " << ec.message() << std::endl; }
            else { std::cout << "Destination path already exists: " << destinationPath << std::endl; }
            return;
        }

        fs::copy(sourcePath, destinationPath, fs::copy_options::recursive, ec);
        if (ec) {
            std::cerr << "Failed to copy directory from " << sourcePath << " to " << destinationPath << ": " << ec.message() << std::endl;
        } else {
            std::cout << "Directory copied from " << sourcePath << " to " << destinationPath << std::endl;
        }
    }

    void create_directory(const std::vector<std::string> &args)
    {
        if (args.size() != 1)
        {
            std::cout << "Usage: create <directory_path>" << std::endl;
            return;
        }
        fs::path dirPath = args[0];
        std::error_code ec;

        if (fs::exists(dirPath, ec))
        {
            if (ec) { std::cerr << "Error checking path existence: " << ec.message() << std::endl; }
            else { std::cout << "Directory already exists: " << dirPath << std::endl; }
            return;
        }
        
        fs::create_directories(dirPath, ec);
        if (ec) {
            std::cerr << "Failed to create directory " << dirPath << ": " << ec.message() << std::endl;
        } else {
            std::cout << "Directory created: " << dirPath << std::endl;
        }
    }

    void delete_directory(const std::vector<std::string> &args)
    {
        if (args.size() != 1)
        {
            std::cout << "Usage: delete <directory_path>" << std::endl;
            return;
        }

        fs::path dirPath = args[0];
        std::error_code ec;

        if (!fs::exists(dirPath, ec))
        {
            if (ec) { std::cerr << "Error checking path existence: " << ec.message() << std::endl; }
            else { std::cout << "Path does not exist: " << dirPath << std::endl; }
            return;
        }
        if (!fs::is_directory(dirPath, ec))
        {
            if (ec) { std::cerr << "Error checking path type: " << ec.message() << std::endl; }
            else { std::cout << "Path is not a directory: " << dirPath << std::endl; }
            return;
        }

        std::cout << "WARNING: This will permanently delete the directory '" << dirPath.string() << "' and all its contents recursively." << std::endl;
        std::cout << "Are you absolutely sure you want to proceed? (yes/no): ";
        std::string confirmation;
        std::cin >> confirmation;
        clear_input_buffer(); 
        std::transform(confirmation.begin(), confirmation.end(), confirmation.begin(), ::tolower); // Chuyển sang chữ thường

        if (confirmation != "yes")
        {
            std::cout << "Deletion cancelled." << std::endl;
            return;
        }
        
        fs::remove_all(dirPath, ec);
        if (ec)
        {
            std::cerr << "Failed to delete directory '" << dirPath.string() << "': " << ec.message() << std::endl;
        }
        else
        {
            std::cout << "Directory '" << dirPath.string() << "' and its contents successfully deleted." << std::endl;
        }
    }

    // Hàm trợ giúp nội bộ cho list_directory_tree
    static void list_directory_tree_helper(const fs::path &path, int level)
    {
        std::error_code ec;

        // Kiểm tra lại path để tránh crash nếu có lỗi trong quá trình duyệt iterator
        if (!fs::exists(path, ec) || !fs::is_directory(path, ec))
        {
            if (ec) {
                std::cerr << "Warning: Could not access directory '" << path.string() << "': " << ec.message() << std::endl;
            }
            return;
        }

        // try-catch cho directory_iterator để bắt lỗi trong quá trình lặp (ví dụ: permission denied)
        try {
            for (auto const& entry : fs::directory_iterator(path, ec))
            {
                if (ec) { // Kiểm tra lỗi sau mỗi lần lấy entry từ iterator
                    std::cerr << "Warning: Error iterating directory '" << path.string() << "': " << ec.message() << std::endl;
                    ec.clear(); // Xóa lỗi để không ảnh hưởng lần lặp tiếp theo
                    continue; // Bỏ qua entry bị lỗi và tiếp tục với entry khác
                }

                for (int i = 0; i < level; ++i)
                {
                    std::cout << "  "; // Thụt lề
                }
                std::cout << "— "; // Dấu gạch nối cho các mục con

                std::cout << entry.path().filename().string();

                // Lấy trạng thái của entry một lần và kiểm tra lỗi
                fs::file_status s = entry.status(ec);
                if (ec) {
                    std::cerr << "Warning: Error getting status for '" << entry.path().string() << "': " << ec.message() << std::endl;
                    std::cout << " (type unknown)" << std::endl;
                    ec.clear(); // Xóa lỗi cho lần lặp tiếp theo
                    continue; // Bỏ qua entry này
                }

                // Dùng s.type() để kiểm tra loại file
                if (s.type() == fs::file_type::directory)
                {
                    std::cout << "/" << std::endl; // Thêm dấu gạch chéo cho thư mục
                    list_directory_tree_helper(entry.path(), level + 1);
                }
                else if (s.type() == fs::file_type::regular)
                {
                    std::cout << std::endl; // File bình thường
                }
                else {
                    std::cout << " (special type)" << std::endl; // Các loại khác (ví dụ: symlink, socket)
                }
                ec.clear(); // Xóa lỗi sau khi xử lý entry để không ảnh hưởng đến lần lặp tiếp theo
            }
        } catch (const fs::filesystem_error& e) {
            // Bắt lỗi nếu directory_iterator ném ra exception (ít xảy ra nếu dùng ec constructor)
            std::cerr << "Filesystem error during iteration of '" << path.string() << "': " << e.what() << std::endl;
        }
    }

    void list_directory_tree(const std::vector<std::string> &args)
    {
        if (args.size() != 1)
        {
            std::cout << "Usage: list_tree <directory_path>" << std::endl;
            return;
        }
        fs::path dirPath = args[0];
        std::error_code ec;

        if (!fs::exists(dirPath, ec))
        {
            if (ec) { std::cerr << "Error checking path existence: " << ec.message() << std::endl; }
            else { std::cout << "Directory does not exist: " << dirPath << std::endl; }
            return;
        }
        if (!fs::is_directory(dirPath, ec)) {
            if (ec) { std::cerr << "Error checking path type: " << ec.message() << std::endl; }
            else { std::cout << "Path is not a directory: " << dirPath << std::endl; }
            return;
        }

        std::cout << dirPath.string() << std::endl;
        // Bọc list_directory_tree_helper trong try-catch để bắt bất kỳ lỗi nào nó ném ra
        try {
            list_directory_tree_helper(dirPath, 1);
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Filesystem error during directory tree listing: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "An unexpected error occurred during directory tree listing: " << e.what() << std::endl;
        }
    }

    void move_directory(const std::vector<std::string> &args)
    {
        if (args.size() != 2)
        {
            std::cout << "Usage: move <source_path> <destination_path>" << std::endl;
            return;
        }

        fs::path sourcePath = args[0];
        fs::path destinationPath = args[1];
        std::error_code ec;

        // 1. Kiểm tra source tồn tại
        if (!fs::exists(sourcePath, ec))
        {
            if (ec) { std::cerr << "Error checking source path existence: " << ec.message() << std::endl; }
            else { std::cout << "Source path does not exist: " << sourcePath << std::endl; }
            return;
        }
        // 2. Kiểm tra source là thư mục
        if (!fs::is_directory(sourcePath, ec))
        {
            if (ec) { std::cerr << "Error checking source path type: " << ec.message() << std::endl; }
            else { std::cout << "Source path is not a directory: " << sourcePath << std::endl; }
            return;
        }

        // 3. Nếu destination là file: lỗi (Dùng s.type() để tương thích tốt hơn)
        fs::file_status dest_status = fs::status(destinationPath, ec);
        if (ec) ec.clear(); // Clear error code from status check if path doesn't exist
        if (fs::exists(dest_status) && fs::is_regular_file(dest_status))
        {
            std::cout << "Destination is a file. Cannot move directory to a file path: " << destinationPath << std::endl;
            return;
        }

        // 4. Nếu destination là thư mục đã tồn tại: lỗi (nếu là thư mục rỗng có thể rename, nhưng đây là "move" nguyên thư mục)
        // std::filesystem::rename chỉ hoạt động nếu đích không tồn tại, hoặc là một thư mục rỗng
        // Nếu đích tồn tại, chúng ta coi là lỗi để đảm bảo an toàn.
        if (fs::exists(destinationPath, ec))
        {
            if (ec) { std::cerr << "Error checking destination path existence: " << ec.message() << std::endl; }
            else { std::cout << "Destination path already exists: " << destinationPath << std::endl; }
            return;
        }

        // 5. Thực hiện di chuyển
        fs::rename(sourcePath, destinationPath, ec);
        if (ec)
        {
            std::cerr << "Failed to move directory from " << sourcePath << " to " << destinationPath << ": " << ec.message() << std::endl;
            // Gợi ý nếu lỗi có thể là do khác ổ đĩa
            if (ec.value() == static_cast<int>(std::errc::cross_device_link))
            {
                std::cerr << "Note: Source and destination may be on different drives. You need to copy then delete manually." << std::endl;
            }
        }
        else
        {
            std::cout << "Directory moved from " << sourcePath << " to " << destinationPath << std::endl;
        }
    }

    // Hàm đổi tên thư mục (alias của move, vì rename trong filesystem thực hiện cả move/rename)
    void rename_directory(const std::vector<std::string> &args) {
        if (args.size() != 2)
        {
            std::cout << "Usage: rename <old_path> <new_path>" << std::endl;
            return;
        }
        move_directory(args); // Gọi hàm move_directory
    }

    // Hàm kiểm tra thư mục có trống không
    void is_directory_empty(const std::vector<std::string>& args) {
        if (args.size() != 1) {
            std::cout << "Usage: is_empty <directory_path>" << std::endl;
            return;
        }
        fs::path dirPath = args[0];
        std::error_code ec;

        if (!fs::exists(dirPath, ec)) {
            if (ec) { std::cerr << "Error checking path existence: " << ec.message() << std::endl; }
            else { std::cout << "Path does not exist: " << dirPath << std::endl; }
            return;
        }
        if (!fs::is_directory(dirPath, ec)) {
            if (ec) { std::cerr << "Error checking path type: " << ec.message() << std::endl; }
            else { std::cout << "Path is not a directory: " << dirPath << std::endl; }
            return;
        }

        if (fs::is_empty(dirPath, ec)) {
            if (ec) {
                std::cerr << "Error checking if directory is empty: " << ec.message() << std::endl;
            } else {
                std::cout << "Directory '" << dirPath.string() << "' is empty." << std::endl;
            }
        } else {
            std::cout << "Directory '" << dirPath.string() << "' is NOT empty." << std::endl;
        }
    }

    // Hàm lấy kích thước thư mục (tính tổng kích thước các file)
    void get_directory_size(const std::vector<std::string>& args) {
        if (args.size() != 1) {
            std::cout << "Usage: get_size <directory_path>" << std::endl;
            return;
        }
        fs::path dirPath = args[0];
        std::error_code ec;

        if (!fs::exists(dirPath, ec)) {
            if (ec) { std::cerr << "Error checking path existence: " << ec.message() << std::endl; }
            else { std::cout << "Path does not exist: " << dirPath << std::endl; }
            return;
        }
        if (!fs::is_directory(dirPath, ec)) {
            if (ec) { std::cerr << "Error checking path type: " << ec.message() << std::endl; }
            else { std::cout << "Path is not a directory: " << dirPath << std::endl; }
            return;
        }

        uintmax_t total_size = 0;
        for (const auto& entry : fs::recursive_directory_iterator(dirPath, ec)) {
            if (ec) { // Kiểm tra lỗi trong khi duyệt
                std::cerr << "Warning: Error accessing entry '" << entry.path().string() << "': " << ec.message() << std::endl;
                ec.clear(); // Xóa lỗi để tiếp tục
                continue;
            }
            if (fs::is_regular_file(entry.status(ec))) { // Chỉ tính kích thước file thường
                if (ec) {
                    std::cerr << "Warning: Error getting file status for '" << entry.path().string() << "': " << ec.message() << std::endl;
                    ec.clear();
                    continue;
                }
                total_size += fs::file_size(entry.path(), ec);
                if (ec) {
                    std::cerr << "Warning: Error getting file size for '" << entry.path().string() << "': " << ec.message() << std::endl;
                    ec.clear();
                    continue;
                }
            }
        }
        if (ec) { // Kiểm tra lỗi sau khi vòng lặp kết thúc
            std::cerr << "An error occurred during directory size calculation: " << ec.message() << std::endl;
        } else {
            std::cout << "Size of directory '" << dirPath.string() << "': " << total_size << " bytes." << std::endl;
        }
    }
}