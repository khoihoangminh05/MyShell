#include "file_commands.h"
#include <iostream>
#include <fstream>      // For std::ifstream, std::ofstream
#include <filesystem>   // For std::filesystem::*, fs::path, fs::file_size, fs::exists
#include <vector>
#include <string>
#include <algorithm>    // For std::min, std::max, std::equal
#include <system_error> // For std::error_code, std::errc
#include <limits>       // For std::numeric_limits
#include <conio.h>      // For _getch (Windows-specific)
#include <csignal>      // For signal (Windows-specific)
#include <windows.h>    // For ShellExecuteA, GetFileAttributesA, SetFileAttributesA, INVALID_FILE_ATTRIBUTES, FILE_ATTRIBUTE_READONLY

namespace fs = std::filesystem;

// Biến tĩnh và hàm xử lý tín hiệu cho đọc file tạm dừng
namespace io{
    static bool interrupted_reading = false;

    static void signalHandler(int signum)
    {
        if (signum == SIGINT) {
            interrupted_reading = true;
        }
    }

    // Hàm trợ giúp để xóa bộ đệm input (có thể dùng chung)
    static void clear_input_buffer() {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Hàm trợ giúp in các dòng (nội bộ của file này)
    static void printLines(const std::vector<std::string> &lines, size_t start, size_t end)
    {
        for (size_t i = start; i < end; ++i)
        {
            std::cout << lines[i] << std::endl;
        }
    }

    // Hàm trợ giúp in các dòng với tạm dừng (nội bộ của file này)
    static void printLinesWithPause(const std::vector<std::string> &lines, size_t start, size_t end)
    {
        interrupted_reading = false; // Reset flag before starting

        for (size_t i = start; i < end; ++i)
        {
            std::cout << lines[i] << std::endl;
            if (interrupted_reading) // Cải tiến: Kiểm tra cờ ngắt tín hiệu
            {
                std::cout << "\nReading interrupted. Returning to shell..." << std::endl;
                return;
            }

            if ((i + 1) % 5 == 0 && i + 1 < end) // Cải tiến: Kiểm tra (i + 1 < end) để tránh hỏi khi đã là dòng cuối cùng
            {
                std::cout << "[READ MORE] (Press any key to continue, Ctrl+C to quit)..." << std::endl;
                char ch = _getch(); // _getch từ <conio.h>
                if (ch == 3) // Ctrl+C (ASCII value)
                { 
                    std::cout << "\nReading interrupted. Returning to shell..." << std::endl;
                    return;
                }
            }
        }
        std::cout << "End of reading. Returning to shell..." << std::endl;
    }

    // --- Các hàm quản lý file đã cải tiến ---

    void write_file(const std::vector<std::string> &args)
    {
        // Cải tiến: Kiểm tra số lượng đối số chặt chẽ hơn và dùng error_code
        if (args.size() < 2 || args.size() > 4)
        {
            std::cerr << "Usage: write_file <content> <filename> [~HEAD | ~FOOT | ~LINE N]" << std::endl;
            return;
        }

        std::string content = args[0];
        fs::path filename = args[1];
        std::error_code ec;

        // Kiểm tra filename có phải là file không
        if (fs::exists(filename, ec) && !fs::is_regular_file(filename, ec)) {
            if (ec) { std::cerr << "Error checking filename type: " << ec.message() << std::endl; }
            else { std::cerr << "Path is not a regular file: " << filename << std::endl; }
            return;
        }

        if (args.size() == 2)
        {
            // Mặc định ghi vào cuối file
            std::ofstream file(filename, std::ios::app);
            if (!file)
            {
                std::cerr << "Failed to open file: " << filename << std::endl;
                return;
            }
            file << content << std::endl;
            file.close();
        }
        else // Có tham số vị trí
        {
            std::string position = args[2];
            std::ifstream file_in(filename);
            if (!file_in)
            {
                std::cerr << "Failed to open file: " << filename << std::endl;
                return;
            }

            std::vector<std::string> lines;
            std::string line;
            while (std::getline(file_in, line))
            {
                lines.push_back(line);
            }
            file_in.close();

            if (position == "~HEAD")
            {
                lines.insert(lines.begin(), content);
            }
            else if (position == "~FOOT")
            {
                lines.push_back(content);
            }
            else if (position == "~LINE")
            {
                if (args.size() != 4)
                {
                    std::cerr << "Usage: write_file <content> <filename> ~LINE N" << std::endl;
                    return;
                }
                try { // Cải tiến: Bắt lỗi stoi
                    size_t line_number = std::stoul(args[3]); // Dùng stoul cho unsigned long
                    if (line_number == 0) { // Dòng số bắt đầu từ 1
                        std::cerr << "Line number must be greater than 0." << std::endl;
                        return;
                    }
                    if (line_number - 1 < lines.size())
                    {
                        lines.insert(lines.begin() + (line_number - 1), content);
                    }
                    else // Nếu dòng số lớn hơn số dòng hiện có, thêm vào cuối
                    {
                        lines.push_back(content);
                    }
                } catch (const std::invalid_argument&) {
                    std::cerr << "Invalid line number: Not a number." << std::endl;
                    return;
                } catch (const std::out_of_range&) {
                    std::cerr << "Line number out of range for integer type." << std::endl;
                    return;
                }
            }
            else
            {
                std::cerr << "Invalid position format. Use ~HEAD, ~FOOT, or ~LINE N." << std::endl;
                return;
            }

            // Ghi lại toàn bộ nội dung đã sửa đổi vào file
            std::ofstream file_out(filename);
            if (!file_out)
            {
                std::cerr << "Failed to open file for writing: " << filename << std::endl;
                return;
            }

            for (const auto &l : lines)
            {
                file_out << l << std::endl;
            }
            file_out.close();
        }
        std::cout << "Successfully wrote to file: " << filename << std::endl;
    }

    void read_file(const std::vector<std::string> &args)
    {
        // Cải tiến: Kiểm tra số lượng đối số chặt chẽ hơn và dùng error_code
        if (args.size() < 1 || args.size() > 4)
        {
            std::cerr << "Usage: read_file <filename> [~HEAD N | ~FOOT N | ~RANGE M N | ~LINE N]" << std::endl;
            return;
        }

        fs::path filename = args[0];
        std::error_code ec;

        if (!fs::exists(filename, ec)) {
            if (ec) { std::cerr << "Error checking file existence: " << ec.message() << std::endl; }
            else { std::cerr << "File does not exist: " << filename << std::endl; }
            return;
        }
        if (!fs::is_regular_file(filename, ec)) {
            if (ec) { std::cerr << "Error checking file type: " << ec.message() << std::endl; }
            else { std::cerr << "Path is not a regular file: " << filename << std::endl; }
            return;
        }

        std::ifstream file(filename);
        if (!file)
        {
            std::cerr << "Could not open file: " << filename << std::endl;
            return;
        }

        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file, line))
        {
            lines.push_back(line);
        }
        file.close();

        // Cải tiến: Đăng ký signal handler tại đây để đảm bảo nó được thiết lập trước khi _getch được gọi
        // signal(SIGINT, signalHandler); // Hàm này chỉ nên gọi 1 lần khi khởi tạo chương trình

        if (args.size() == 1)
        {
            // Mặc định đọc toàn bộ file, 5 dòng một lần và hỏi người dùng ấn Enter để tiếp tục hoặc Ctrl+C để thoát
            printLinesWithPause(lines, 0, lines.size());
        }
        else // Có tham số vị trí
        {
            std::string position = args[1];
            try { // Cải tiến: Bắt lỗi stoi/stoul
                if (position == "~HEAD")
                {
                    if (args.size() != 3) { std::cerr << "Usage: read_file <filename> ~HEAD N" << std::endl; return; }
                    size_t num_lines = std::stoul(args[2]);
                    printLines(lines, 0, std::min(num_lines, lines.size()));
                }
                else if (position == "~FOOT")
                {
                    if (args.size() != 3) { std::cerr << "Usage: read_file <filename> ~FOOT N" << std::endl; return; }
                    size_t num_lines = std::stoul(args[2]);
                    printLines(lines, std::max((int)lines.size() - (int)num_lines, 0), lines.size()); // Cải tiến: ép kiểu int để tránh lỗi với size_t
                }
                else if (position == "~RANGE")
                {
                    if (args.size() != 4) { std::cerr << "Usage: read_file <filename> ~RANGE M N" << std::endl; return; }
                    size_t start_line = std::stoul(args[2]) - 1;
                    size_t end_line = std::stoul(args[3]) - 1; // Cải tiến: end_line là chỉ số, không phải số dòng
                    if (start_line > end_line) { // Cải tiến: Bắt lỗi start > end
                        std::cerr << "Invalid line range: Start line cannot be greater than end line." << std::endl;
                        return;
                    }
                    if (start_line < lines.size() && end_line < lines.size()) // Cải tiến: Kiểm tra cả end_line
                    {
                        printLines(lines, start_line, end_line + 1); // Cải tiến: +1 vì end_line là chỉ số
                    }
                    else
                    {
                        std::cerr << "Invalid line range: Out of bounds." << std::endl;
                    }
                }
                else if (position == "~LINE")
                {
                    if (args.size() != 3) { std::cerr << "Usage: read_file <filename> ~LINE N" << std::endl; return; }
                    size_t line_number = std::stoul(args[2]) - 1;
                    if (line_number < lines.size())
                    {
                        printLines(lines, line_number, line_number + 1);
                    }
                    else
                    {
                        std::cerr << "Line number out of range." << std::endl;
                    }
                }
                else
                {
                    std::cerr << "Invalid position format." << std::endl;
                }
            } catch (const std::invalid_argument&) {
                std::cerr << "Invalid number format for line count or range." << std::endl;
            } catch (const std::out_of_range&) {
                std::cerr << "Number out of range." << std::endl;
            }
        }
    }

    void show_file_info(const std::vector<std::string> &args) // Đổi tên
    {
        if (args.size() != 1)
        {
            std::cout << "Usage: file_info <filename>" << std::endl;
            return;
        }
        fs::path fileName = args[0];
        std::error_code ec;

        if (!fs::exists(fileName, ec)) {
            if (ec) { std::cerr << "Error checking file existence: " << ec.message() << std::endl; }
            else { std::cerr << "File does not exist: " << fileName << std::endl; }
            return;
        }
        if (!fs::is_regular_file(fileName, ec)) {
            if (ec) { std::cerr << "Error checking file type: " << ec.message() << std::endl; }
            else { std::cerr << "Path is not a regular file: " << fileName << std::endl; }
            return;
        }

        try
        {
            auto fileSize = fs::file_size(fileName, ec); // Cải tiến: dùng error_code
            if (ec) { std::cerr << "Error getting file size: " << ec.message() << std::endl; return; }

            std::ifstream file(fileName);
            if (!file) { std::cerr << "Failed to open file for line count: " << fileName << std::endl; return; }
            
            size_t lineCount = 0;
            std::string line;
            while (std::getline(file, line))
            {
                lineCount++;
            }
            file.close();

            std::cout << "Size of file '" << fileName.string() << "': " << fileSize << " bytes" << std::endl;
            std::cout << "Number of lines in file '" << fileName.string() << "': " << lineCount << std::endl;
        }
        catch (const fs::filesystem_error &e) // Cải tiến: Bắt lỗi nếu vẫn có exception (ít xảy ra với ec)
        {
            std::cerr << "Filesystem error: " << e.what() << std::endl;
        }
        catch (const std::exception& e) { // Cải tiến: Bắt lỗi chung
            std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
        }
    }

    void open_path(const std::vector<std::string> &args) // Đổi tên
    {
        if (args.size() != 1)
        {
            std::cout << "Usage: open <file_or_folder_path>" << std::endl;
            return;
        }

        std::string path_str = args[0]; // Giữ là string vì ShellExecuteA cần const char*
        std::error_code ec;
        fs::path p = path_str;

        if (!fs::exists(p, ec)) // Cải tiến: Kiểm tra tồn tại với error_code
        {
            if (ec) { std::cerr << "Error checking path existence: " << ec.message() << std::endl; }
            else { std::cout << "Path does not exist: " << p << std::endl; }
            return;
        }

        // ShellExecuteA không trả về error_code, chỉ SUCCESS hoặc ERROR_CODE > 32
        INT_PTR shell_result = (INT_PTR)ShellExecuteA(NULL, "open", path_str.c_str(), NULL, NULL, SW_SHOW);
        // So sánh với 32
        if (shell_result <= 32) { // ERROR_CODE <= 32 là lỗi
            std::cerr << "Failed to open path: " << path_str << " (Error code: " << shell_result << ")" << std::endl;
        } else {
            std::cout << "Opened path: " << path_str << std::endl;
        }
    }

    void rename_file(const std::vector<std::string> &args) // Đổi tên
    {
        if (args.size() != 2)
        {
            std::cout << "Usage: rename_file <old_file_path> <new_file_path>" << std::endl;
            return;
        }
        fs::path oldPath = args[0];
        fs::path newPath = args[1];
        std::error_code ec;

        // Cải tiến: Kiểm tra tồn tại và loại file với error_code
        if (!fs::exists(oldPath, ec)) {
            if (ec) { std::cerr << "Error checking old path existence: " << ec.message() << std::endl; }
            else { std::cerr << "Old file does not exist: " << oldPath << std::endl; }
            return;
        }
        if (!fs::is_regular_file(oldPath, ec)) {
            if (ec) { std::cerr << "Error checking old path type: " << ec.message() << std::endl; }
            else { std::cerr << "Old path is not a regular file: " << oldPath << std::endl; }
            return;
        }
        if (fs::exists(newPath, ec)) {
            if (ec) { std::cerr << "Error checking new path existence: " << ec.message() << std::endl; }
            else { std::cerr << "New path already exists: " << newPath << std::endl; }
            return;
        }

        fs::rename(oldPath, newPath, ec); // Cải tiến: dùng error_code
        if (ec)
        {
            std::cerr << "Failed to rename file from '" << oldPath.string() << "' to '" << newPath.string() << "': " << ec.message() << std::endl;
        }
        else
        {
            std::cout << "File renamed from " << oldPath << " to " << newPath << std::endl;
        }
    }

    void create_file_command(const std::vector<std::string> &args) // Đổi tên
    {
        if (args.empty()) // Cải tiến: Có thể tạo nhiều file
        {
            std::cerr << "Usage: touch <file1> [<file2> ...]" << std::endl;
            return;
        }
        for (const auto &fileName : args)
        {
            fs::path filePath = fileName;
            std::error_code ec;

            if (fs::exists(filePath, ec)) { // Cải tiến: Kiểm tra đã tồn tại
                if (ec) { std::cerr << "Error checking path existence: " << ec.message() << std::endl; }
                else { std::cout << "File already exists: " << filePath << std::endl; }
                continue; // Tiếp tục tạo các file khác
            }

            std::ofstream file(filePath); // Mở file để tạo (nếu không tồn tại) hoặc ghi đè (nếu tồn tại)
            if (file.is_open()) // Cải tiến: Kiểm tra is_open()
            {
                file.close();
                std::cout << "File created successfully: " << filePath << std::endl;
            }
            else
            {
                std::cerr << "Failed to create file: " << filePath << std::endl;
            }
        }
    }

    void delete_file_command(const std::vector<std::string> &args) // Đổi tên
    {
        if (args.empty()) // Cải tiến: Có thể xóa nhiều file
        {
            std::cerr << "Usage: rm <file1> [<file2> ...]" << std::endl;
            return;
        }
        for (const auto &fileName : args)
        {
            fs::path filePath = fileName;
            std::error_code ec;

            if (!fs::exists(filePath, ec)) { // Cải tiến: Kiểm tra tồn tại
                if (ec) { std::cerr << "Error checking path existence: " << ec.message() << std::endl; }
                else { std::cerr << "File does not exist: " << filePath << std::endl; }
                continue;
            }
            if (!fs::is_regular_file(filePath, ec)) { // Cải tiến: Kiểm tra là file thường
                if (ec) { std::cerr << "Error checking path type: " << ec.message() << std::endl; }
                else { std::cerr << "Path is not a regular file: " << filePath << std::endl; }
                continue;
            }

            if (fs::remove(filePath, ec)) // Cải tiến: dùng error_code
            {
                std::cout << "File deleted successfully: " << filePath << std::endl;
            }
            else
            {
                std::cerr << "Failed to delete file '" << filePath.string() << "': " << ec.message() << std::endl;
            }
        }
    }

    void check_file_existence(const std::vector<std::string> &args) // Đổi tên
    {
        if (args.empty()) // Cải tiến: Có thể kiểm tra nhiều file
        {
            std::cerr << "Usage: exists <file1> [<file2> ...]" << std::endl;
            return;
        }
        for (const auto &fileName : args)
        {
            fs::path filePath = fileName;
            std::error_code ec;

            if (fs::exists(filePath, ec)) // Cải tiến: dùng error_code
            {
                if (ec) { std::cerr << "Error checking file existence: " << ec.message() << std::endl; }
                else { std::cout << "File exists: " << filePath << std::endl; }
            }
            else
            {
                if (ec) { std::cerr << "Error checking file existence: " << ec.message() << std::endl; }
                else { std::cout << "File does not exist: " << filePath << std::endl; }
            }
        }
    }
    void print_file_extensions(const std::vector<std::string> &args) // Đổi tên
    {
        if (args.empty()) // Cải tiến: Có thể in nhiều file
        {
            std::cerr << "Usage: extensions <file1> [<file2> ...]" << std::endl;
            return;
        }
        for (const auto &fileName : args)
        {
            fs::path filePath = fileName;
            std::error_code ec;

            if (!fs::exists(filePath, ec)) {
                if (ec) { std::cerr << "Error checking file existence: " << ec.message() << std::endl; }
                else { std::cerr << "File does not exist: " << filePath << std::endl; }
                continue;
            }
            if (!fs::is_regular_file(filePath, ec)) {
                if (ec) { std::cerr << "Error checking file type: " << ec.message() << std::endl; }
                else { std::cerr << "Path is not a regular file: " << filePath << std::endl; }
                continue;
            }

            std::cout << "Extension of file '" << filePath.string() << "': " << filePath.extension().string() << std::endl; // Cải tiến: dùng .string()
        }
    }

    void copy_file_command(const std::vector<std::string> &args) // Đổi tên
    {
        if (args.size() != 2)
        {
            std::cout << "Usage: copy_file <source_file_path> <destination_file_path>" << std::endl;
            return;
        }
        fs::path sourcePath = args[0];
        fs::path destinationPath = args[1];
        std::error_code ec;

        // Cải tiến: Kiểm tra tồn tại và loại file với error_code
        if (!fs::exists(sourcePath, ec)) {
            if (ec) { std::cerr << "Error checking source existence: " << ec.message() << std::endl; }
            else { std::cerr << "Source file does not exist: " << sourcePath << std::endl; }
            return;
        }
        if (!fs::is_regular_file(sourcePath, ec)) {
            if (ec) { std::cerr << "Error checking source type: " << ec.message() << std::endl; }
            else { std::cerr << "Source path is not a regular file: " << sourcePath << std::endl; }
            return;
        }
        if (fs::exists(destinationPath, ec)) { // Cải tiến: Kiểm tra đích có tồn tại không
            if (ec) { std::cerr << "Error checking destination existence: " << ec.message() << std::endl; }
            else { std::cerr << "Destination file already exists: " << destinationPath << std::endl; }
            return;
        }

        fs::copy_file(sourcePath, destinationPath, ec); // Cải tiến: dùng error_code
        if (ec)
        {
            std::cerr << "Failed to copy file from '" << sourcePath.string() << "' to '" << destinationPath.string() << "': " << ec.message() << std::endl;
        }
        else
        {
            std::cout << "File copied successfully from " << sourcePath << " to " << destinationPath << std::endl;
        }
    }

    void move_file_command(const std::vector<std::string> &args) // Đổi tên
    {
        if (args.size() != 2)
        {
            std::cout << "Usage: move_file <source_file_path> <destination_file_path>" << std::endl;
            return;
        }
        fs::path sourcePath = args[0];
        fs::path destinationPath = args[1];
        std::error_code ec;

        // Cải tiến: Kiểm tra tồn tại và loại file với error_code
        if (!fs::exists(sourcePath, ec)) {
            if (ec) { std::cerr << "Error checking source existence: " << ec.message() << std::endl; }
            else { std::cerr << "Source file does not exist: " << sourcePath << std::endl; }
            return;
        }
        if (!fs::is_regular_file(sourcePath, ec)) {
            if (ec) { std::cerr << "Error checking source type: " << ec.message() << std::endl; }
            else { std::cerr << "Source path is not a regular file: " << sourcePath << std::endl; }
            return;
        }
        if (fs::exists(destinationPath, ec)) { // Cải tiến: Kiểm tra đích có tồn tại không
            if (ec) { std::cerr << "Error checking destination existence: " << ec.message() << std::endl; }
            else { std::cerr << "Destination file already exists: " << destinationPath << std::endl; }
            return;
        }

        fs::rename(sourcePath, destinationPath, ec); // Cải tiến: dùng error_code
        if (ec)
        {
            std::cerr << "Failed to move file from '" << sourcePath.string() << "' to '" << destinationPath.string() << "': " << ec.message() << std::endl;
            // Gợi ý nếu lỗi có thể là do khác ổ đĩa
            if (ec.value() == static_cast<int>(std::errc::cross_device_link)) {
                std::cerr << "Note: Source and destination may be on different drives. You need to copy then delete manually." << std::endl;
            }
        }
        else
        {
            std::cout << "File moved successfully from " << sourcePath << " to " << destinationPath << std::endl;
        }
    }

    void list_files_with_extension(const std::vector<std::string> &args) // Đổi tên
    {
        if (args.size() < 1 || args.size() > 2)
        {
            std::cout << "Usage: list_ext [directory] <extension>" << std::endl; // Cải tiến: Usage
            return;
        }

        fs::path directory = ".";
        std::string extension = "";

        if (args.size() == 1) { // list_ext <extension> (in current dir)
            extension = args[0];
        } else { // list_ext <directory> <extension>
            directory = args[0];
            extension = args[1];
        }

        std::error_code ec;
        if (!fs::exists(directory, ec) || !fs::is_directory(directory, ec)) {
            if (ec) { std::cerr << "Error checking directory: " << ec.message() << std::endl; }
            else { std::cerr << "Directory does not exist or is not a directory: " << directory << std::endl; }
            return;
        }

        std::cout << "Files with extension '" << extension << "' in '" << directory.string() << "':\n";
        try {
            for (const auto &entry : fs::directory_iterator(directory, ec))
            {
                if (ec) { std::cerr << "Warning: Error iterating directory: " << ec.message() << std::endl; ec.clear(); continue; }
                
                if (fs::is_regular_file(entry.status(ec))) { // Cải tiến: Kiểm tra là file thường
                    if (ec) { std::cerr << "Warning: Error getting file status: " << ec.message() << std::endl; ec.clear(); continue; }

                    if (entry.path().extension() == extension) // Cải tiến: So sánh extension
                    {
                        std::cout << entry.path().filename().string() << std::endl;
                    }
                }
            }
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Filesystem error listing files with extension: " << e.what() << std::endl;
        }
    }

    // Cải tiến: Đưa hàm compare_files_command vào đây
    void compare_files(const std::vector<std::string>& args) {
        if (args.size() != 2) {
            std::cout << "Usage: compare <file1_path> <file2_path>\n";
            return;
        }
        fs::path file1_path = args[0];
        fs::path file2_path = args[1];
        std::error_code ec;

        if (!fs::exists(file1_path, ec) || !fs::is_regular_file(file1_path, ec)) {
            if (ec) { std::cerr << "Error checking file1: " << ec.message() << std::endl; }
            else { std::cout << "File 1 does not exist or is not a regular file: " << file1_path << std::endl; }
            return;
        }
        if (!fs::exists(file2_path, ec) || !fs::is_regular_file(file2_path, ec)) {
            if (ec) { std::cerr << "Error checking file2: " << ec.message() << std::endl; }
            else { std::cout << "File 2 does not exist or is not a regular file: " << file2_path << std::endl; }
            return;
        }

        std::ifstream f1(file1_path, std::ifstream::binary);
        std::ifstream f2(file2_path, std::ifstream::binary);

        if (f1.fail() || f2.fail()) {
            std::cerr << "Error opening one or both files for comparison.\n";
            return;
        }

        uintmax_t size1 = fs::file_size(file1_path, ec);
        if (ec) { std::cerr << "Error getting size of file1: " << ec.message() << std::endl; return; }
        uintmax_t size2 = fs::file_size(file2_path, ec);
        if (ec) { std::cerr << "Error getting size of file2: " << ec.message() << std::endl; return; }

        if (size1 != size2) {
            std::cout << "Files are DIFFERENT (sizes differ).\n";
            return;
        }

        if (std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
                    std::istreambuf_iterator<char>(),
                    std::istreambuf_iterator<char>(f2.rdbuf())))
        {
            std::cout << "Files are IDENTICAL.\n";
        }
        else
        {
            std::cout << "Files are DIFFERENT (content differs).\n";
        }

        f1.close();
        f2.close();
    }

    // Cải tiến: Đưa hàm get_absolute_path_command vào đây
    void get_absolute_path_command(const std::vector<std::string>& args) {
        if (args.size() != 1) {
            std::cout << "Usage: abspath <path>\n";
            return;
        }
        fs::path p = args[0];
        std::error_code ec;
        fs::path absolutePath = fs::absolute(p, ec);

        if (ec) {
            std::cerr << "Failed to get absolute path for '" << p.string() << "': " << ec.message() << std::endl;
        } else {
            std::cout << "Absolute Path: " << absolutePath.string() << "\n";
        }
    }

    // Cải tiến: Đưa hàm get_normalized_path_command vào đây
    void get_normalized_path_command(const std::vector<std::string>& args) {
        if (args.size() != 1) {
            std::cout << "Usage: normalize <path>\n";
            return;
        }
        fs::path p = args[0];
        std::error_code ec;
        fs::path normalizedPath = fs::weakly_canonical(p, ec);

        if (ec) {
            std::cerr << "Failed to normalize path '" << p.string() << "': " << ec.message() << std::endl;
        } else {
            std::cout << "Normalized Path: " << normalizedPath.string() << "\n";
        }
    }
}