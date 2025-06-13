#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>

void clearLines(int count) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD cursorPos;
    
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        cursorPos = csbi.dwCursorPosition;
        cursorPos.Y -= count;
        SetConsoleCursorPosition(hConsole, cursorPos);
    }
}

int main() {
    const char* spinner[] = {
        " O \n" 
        "/|\\\n"
        "/ \\",
        
        " O \n" 
        "-|-\n"
        "/ \\",
        
        " O \n" 
        "\\|/\n"
        "/ \\",
        
        " O \n" 
        "-|-\n"
        "/ \\"
    };
    
    const int spinner_size = sizeof(spinner) / sizeof(spinner[0]);
    int counter = 20000;  // Số lần quay
    
    while (counter--) {
        std::cout << spinner[counter % spinner_size] << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        clearLines(3);  // Di chuyển con trỏ lên 3 dòng
    }
    
    // In thêm dòng trống để tránh ghi đè
    std::cout << "\n\nHoan tat!" << std::endl;
    
    return 0;
}