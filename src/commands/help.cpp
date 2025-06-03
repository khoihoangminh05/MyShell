#include "help.h"
#include <iostream>

namespace commands {

void print_section_header(const std::string& title) {
    std::cout << "\n=== " << title << " ===\n";
}

void help() {

    // 1. General commands
    print_section_header("General Commands");
    std::cout << "  help                    Show this help menu\n";
    std::cout << "  exit                    Exit the shell\n";
    std::cout << "  clear                   Clear the screen\n";
    std::cout << "  calc                    Open system calculator\n";
    std::cout << "  run <script>            Run a script file\n";

    // 2. Shell Utilities
    print_section_header("Shell Utility Commands");
    std::cout << "  setcolor <code>         Change shell text color\n";
    std::cout << "  sys_info                Show all system information\n";
    std::cout << "  date                    Display current date\n";
    std::cout << "  time                    Display current time\n";
    std::cout << "  cpu_info                Display CPU information\n";
    std::cout << "  memory_info             Display RAM information\n";
    std::cout << "  disk_info               Display disk storage info\n";
    std::cout << "  computer_name           Show computer name\n";
    std::cout << "  os_info                 Show OS information\n";

    // 3. Directory management
    print_section_header("Directory Commands");
    std::cout << "  copy <src> <dest>       Copy directory\n";
    std::cout << "  create <name>           Create directory\n";
    std::cout << "  delete <name>           Delete directory\n";
    std::cout << "  list_tree <path>        Show directory tree\n";
    std::cout << "  move <src> <dest>       Move directory\n";
    std::cout << "  rename <old> <new>      Rename directory\n";
    std::cout << "  is_empty <dir>          Check if directory is empty\n";
    std::cout << "  get_size <dir>          Get size of directory\n";

    // 4. File management
    print_section_header("File Commands");
    std::cout << "  touch <file>            Create file\n";
    std::cout << "  rm <file>               Delete file\n";
    std::cout << "  rename_file <old> <new> Rename file\n";
    std::cout << "  copy_file <src> <dest>  Copy file\n";
    std::cout << "  move_file <src> <dest>  Move file\n";
    std::cout << "  list_ext <ext>          List files with extension\n";
    std::cout << "  extensions <dir>        List file extensions in folder\n";
    std::cout << "  file_info <file>        Show file metadata\n";
    std::cout << "  open <path>             Open file or folder\n";
    std::cout << "  exists <path>           Check if path exists\n";
    std::cout << "  abspath <rel>           Show absolute path\n";
    std::cout << "  normalize <path>        Normalize path\n";
    std::cout << "  compare <f1> <f2>       Compare two files\n";

    // 5. Navigation
    print_section_header("Navigation Commands");
    std::cout << "  pwd                     Print current directory\n";
    std::cout << "  cd <path>               Change current directory\n";
    std::cout << "  ls                      List directory contents\n";

    //6. Environment Management
    print_section_header("Environment Management");
    std::cout << "  add_path <dir>          Add a directory to PATH\n";
    std::cout << "  remove_path <dir>       Remove a directory from PATH\n";
    std::cout << "  is_in_path <dir>        Check if directory is in PATH\n";
    std::cout << "  set_env <var> <value>   Set environment variable\n";
    std::cout << "  get_env <var>           Get value of environment variable\n";
    std::cout << "  print_env <var>         Print environment variable value\n";
    std::cout << "  unset_env <var>         Unset (remove) environment variable\n";
    std::cout << "  delay_command <args>    Schedule a command to run later\n";
    // std::cout << "  list_env                List all environment variables\n";
    // std::cout << "  save_env <file>         Save current environment to file\n";
    // std::cout << "  load_env <file>         Load environment variables from file\n";


        // 7. Process Management
    print_section_header("Process Management");
    std::cout << "  list_proc               List all running processes\n";
    std::cout << "  terminate <pid>         Terminate a process by PID\n";
    std::cout << "  suspend <pid>           Suspend a process by PID\n";
    std::cout << "  resume <pid>            Resume a suspended process\n";

    // 8. Fun & Utilities
    print_section_header("Fun & Utilities");
    std::cout << "  countdown [n]           Run countdown timer (default 10s)\n";
    std::cout << "  spinner                 Run loading spinner animation\n";
    std::cout << "  tictactoe               Play Tic Tac Toe game\n";
    std::cout << "  logger                  Run background logger\n";

    // 9. Starting External Commands
    print_section_header("Starting External Commands");
    std::cout << "  start <command> [&]     Start an external process\n";
    std::cout << "                         You can omit 'start' and just type the command\n";
    std::cout << "                         Add '&' at the end to run it in background\n";
    std::cout << "                         Examples:\n";
    std::cout << "                           start notepad.exe\n";
    std::cout << "                           ping google.com &\n";

    // 10. Background Processes
    print_section_header("Background Processes");
    std::cout << "  command &               Run any command in background\n";
    std::cout << "                         The shell will not wait for it to finish\n";
    std::cout << "                         Use 'terminate <pid>' to stop background processes\n";
    std::cout << "\n🔚 Tip: Ctrl+C only stops foreground processes\n";


}

}
