// countdown.cpp
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>

int main(int argc, char* argv[]) {
    int seconds = 10;
    if (argc > 1) seconds = std::atoi(argv[1]);

    for (int i = seconds; i > 0; --i) {
        std::cout << "Countdown: " << i << "...\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "Time's up!\n";
    return 0;
}
