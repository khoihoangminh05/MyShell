#include "system_utils.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <string>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

namespace sysutils {

    void print_current_date() {
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::tm* now_tm = std::localtime(&t);

        std::cout << "Date: " 
                  << now_tm->tm_mday << "/" 
                  << now_tm->tm_mon + 1 << "/" 
                  << now_tm->tm_year + 1900 << std::endl;
    }

    void print_current_time() {
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::tm* now_tm = std::localtime(&t);

        std::cout << "Time: "
                  << now_tm->tm_hour << ":" 
                  << now_tm->tm_min << ":" 
                  << now_tm->tm_sec << std::endl;
    }

    void print_cpu_info() {
#ifdef _WIN32
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        std::cout << "CPU Cores: " << sysInfo.dwNumberOfProcessors << std::endl;
#else
        std::cout << "CPU Info: " << std::flush;
        system("lscpu | grep 'Model name'");
#endif
    }

    void print_memory_info() {
#ifdef _WIN32
        MEMORYSTATUSEX status;
        status.dwLength = sizeof(status);
        GlobalMemoryStatusEx(&status);
        std::cout << "Total RAM: " << status.ullTotalPhys / (1024 * 1024) << " MB" << std::endl;
#else
        system("free -h");
#endif
    }

    void print_disk_info() {
#ifdef _WIN32
        ULARGE_INTEGER freeBytesAvailable, totalBytes, totalFreeBytes;
        GetDiskFreeSpaceEx(NULL, &freeBytesAvailable, &totalBytes, &totalFreeBytes);
        std::cout << "Total Disk Space: " << totalBytes.QuadPart / (1024 * 1024 * 1024) << " GB" << std::endl;
#else
        system("df -h");
#endif
    }

    void print_computer_name() {
        char name[256];
#ifdef _WIN32
        DWORD size = sizeof(name);
        GetComputerName(name, &size);
#else
        gethostname(name, sizeof(name));
#endif
        std::cout << "Computer Name: " << name << std::endl;
    }

    void print_os_info() {
#ifdef _WIN32
        std::cout << "OS: Windows" << std::endl;
#else
        system("uname -a");
#endif
    }

    void print_all_system_info() {
        print_computer_name();
        print_os_info();
        print_cpu_info();
        print_memory_info();
        print_disk_info();
        print_current_date();
        print_current_time();
    }

}
