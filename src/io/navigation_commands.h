#pragma once
#ifndef NAVIGATION_COMMANDS_H
#define NAVIGATION_COMMANDS_H

#include <string>
#include <vector>

namespace io {

    // --- Các hàm điều hướng thư mục ---

    // Thay đổi thư mục hiện hành (cd)
    void change_directory(const std::vector<std::string> &args);

    // Liệt kê nội dung thư mục (ls)
    void list_directory_contents(const std::vector<std::string> &args);

    // In đường dẫn thư mục hiện hành (pwd)
    void print_working_directory(const std::vector<std::string> &args);

} // namespace commands

#endif // NAVIGATION_COMMANDS_H