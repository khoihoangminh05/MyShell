#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <ctime>

int main() {
    std::ofstream logfile("log.txt", std::ios::app);
    if (!logfile) {
        std::cerr << "Unable to open log.txt\n";
        return 1;
    }

    while (true) {
        auto now = std::time(nullptr);
        logfile << "Logged at: " << std::ctime(&now);
        logfile.flush();
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    return 0;
}
