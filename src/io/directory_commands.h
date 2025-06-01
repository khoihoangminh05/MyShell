#pragma once
#ifndef DIRECTORY_COMMANDS_H
#define DIRECTORY_COMMANDS_H

#include <string>
#include <vector>
#include <unordered_set> // Cho supportedCommands

namespace io {

    // Bộ sưu tập các lệnh quản lý thư mục được hỗ trợ
    extern const std::unordered_set<std::string> directory_commands;

    // --- Các hàm quản lý thư mục ---

    // Hàm sao chép thư mục
    void copy_directory(const std::vector<std::string> &args);

    // Hàm tạo thư mục
    void create_directory(const std::vector<std::string> &args);

    // Hàm xóa thư mục (đệ quy)
    void delete_directory(const std::vector<std::string> &args);

    // Hàm liệt kê cây thư mục
    void list_directory_tree(const std::vector<std::string> &args);

    // Hàm di chuyển thư mục (đổi tên)
    void move_directory(const std::vector<std::string> &args);

    // Hàm đổi tên thư mục (tên khác của moveDirectory nếu muốn)
    void rename_directory(const std::vector<std::string> &args);

    // Hàm kiểm tra thư mục có trống không
    void is_directory_empty(const std::vector<std::string>& args);

    // Hàm lấy kích thước thư mục
    void get_directory_size(const std::vector<std::string>& args);
} // namespace commands

#endif // DIRECTORY_COMMANDS_H