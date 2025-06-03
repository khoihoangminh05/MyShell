#pragma once
#ifndef FILE_COMMANDS_H
#define FILE_COMMANDS_H

#include <string>
#include <vector>

namespace io {

    // --- Các hàm quản lý file ---

    // Ghi nội dung vào file
    void write_file(const std::vector<std::string> &args);

    // Đọc file
    void read_file(const std::vector<std::string> &args);

    // Lấy kích thước file và số lượng dòng
    void show_file_info(const std::vector<std::string> &args); // Đổi tên từ showFileSize

    // Mở file/folder bằng ứng dụng mặc định
    void open_path(const std::vector<std::string> &args); // Đổi tên từ openPath

    // Đổi tên file
    void rename_file(const std::vector<std::string> &args); // Đổi tên từ renameFile

    // Tạo file mới
    void create_file_command(const std::vector<std::string> &args); // Đổi tên từ createFile

    // Xóa file
    void delete_file_command(const std::vector<std::string> &args); // Đổi tên từ deleteFile

    // Kiểm tra file có tồn tại không
    void check_file_existence(const std::vector<std::string> &args); // Đổi tên từ checkFileExistence

    // In ra phần mở rộng của file
    void print_file_extensions(const std::vector<std::string> &args); // Đổi tên từ printFileExtensions

    // Sao chép file
    void copy_file_command(const std::vector<std::string> &args); // Đổi tên từ copyFile

    // Di chuyển file
    void move_file_command(const std::vector<std::string> &args); // Đổi tên từ moveFile

    // Liệt kê file có đuôi xác định
    void list_files_with_extension(const std::vector<std::string> &args); // Đổi tên từ listFilesWithExtension

    // So sánh nội dung 2 file
    void compare_files(const std::vector<std::string>& args); // Đổi tên từ compare_files_command

    // Lấy đường dẫn tuyệt đối
    void get_absolute_path_command(const std::vector<std::string>& args); // Đổi tên từ get_absolute_path_command

    // Lấy đường dẫn chuẩn hóa (normalize)
    void get_normalized_path_command(const std::vector<std::string>& args); // Đổi tên từ get_normalized_path

} // namespace commands

#endif // FILE_COMMANDS_H