#pragma once
#ifndef COMMANDS_RUN_SCRIPT_H
#define COMMANDS_RUN_SCRIPT_H

#include <string>
#include <vector>
#include <filesystem> // std::filesystem::path

namespace commands {

    // Hàm giải quyết đường dẫn file script hoặc chương trình
    std::filesystem::path resolve_path(const std::string &path_str);

    // Hàm chạy script
    void run_script(const std::vector<std::string> &args);

} // namespace commands

#endif // COMMANDS_RUN_SCRIPT_H