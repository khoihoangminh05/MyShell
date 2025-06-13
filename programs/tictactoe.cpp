#include <iostream>
#include <cctype>
#include <limits>

char board[3][3] = {{' ', ' ', ' '}, 
                    {' ', ' ', ' '}, 
                    {' ', ' ', ' '}};
char currentPlayer = 'X';

void printBoard() {
    std::cout << "\n  0   1   2\n"; // Cột
    for (int i = 0; i < 3; ++i) {
        std::cout << i << " "; // Hàng
        for (int j = 0; j < 3; ++j) {
            std::cout << board[i][j];
            if (j < 2) std::cout << " | ";
        }
        if (i < 2) std::cout << "\n  ---------\n";
    }
    std::cout << "\n\n";
}

bool makeMove(int row, int col) {
    if (row < 0 || row > 2 || col < 0 || col > 2) return false;
    if (board[row][col] != ' ') return false;
    
    board[row][col] = currentPlayer;
    return true;
}

bool checkWin() {
    // Kiểm tra hàng và cột
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2]) return true;
        if (board[0][i] != ' ' && board[0][i] == board[1][i] && board[1][i] == board[2][i]) return true;
    }
    
    // Kiểm tra đường chéo
    if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2]) return true;
    if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0]) return true;
    
    return false;
}

bool checkDraw() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == ' ') return false;
        }
    }
    return true;
}

void switchPlayer() {
    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
}

void resetGame() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            board[i][j] = ' ';
        }
    }
    currentPlayer = 'X';
}

int main() {
    std::cout << "Tic Tac Toe is launching...\n";
    bool playAgain = true;
    
    while (playAgain) {
        resetGame();
        bool gameEnded = false;
        
        while (!gameEnded) {
            printBoard();
            int row, col;
            
            // Nhập liệu
            while (true) {
                std::cout << "Player " << currentPlayer << ", enter row (0-2) and column (0-2): ";
                if (std::cin >> row >> col) {
                    if (makeMove(row, col)) break;
                    else std::cout << "Invalid move! Try again.\n";
                } else {
                    std::cout << "Invalid input! Please enter numbers only.\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }
            
            // Kiểm tra kết quả
            if (checkWin()) {
                printBoard();
                std::cout << "Player " << currentPlayer << " wins!\n";
                gameEnded = true;
            } else if (checkDraw()) {
                printBoard();
                std::cout << "It's a draw!\n";
                gameEnded = true;
            } else {
                switchPlayer();
            }
        }
        
        // Hỏi chơi lại
        char response;
        std::cout << "Play again? (y/n): ";
        std::cin >> response;
        playAgain = (std::tolower(response) == 'y');
    }
    
    std::cout << "\nThanks for playing!\n";
    return 0;
}