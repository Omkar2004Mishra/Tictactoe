#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

char board[3][3];
char player = 'X';
char ai = 'O';

// Initialize the board with empty spaces
void resetBoard() {
    char count = '1';
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = count++;
}

// Display the board
void printBoard() {
    cout << "\n";
    for (int i = 0; i < 3; i++) {
        cout << " ";
        for (int j = 0; j < 3; j++) {
            cout << board[i][j];
            if (j < 2) cout << " | ";
        }
        cout << "\n";
        if (i < 2) cout << "---+---+---\n";
    }
    cout << "\n";
}

// Check if there's a winner
char checkWinner() {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] &&
            board[i][1] == board[i][2])
            return board[i][0];
        if (board[0][i] == board[1][i] &&
            board[1][i] == board[2][i])
            return board[0][i];
    }
    if (board[0][0] == board[1][1] &&
        board[1][1] == board[2][2])
        return board[0][0];
    if (board[0][2] == board[1][1] &&
        board[1][1] == board[2][0])
        return board[0][2];
    return ' ';
}

// Check if the board is full
bool isBoardFull() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] != 'X' && board[i][j] != 'O')
                return false;
    return true;
}

// Check if a player can win in the next move
bool canWin(char symbol) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != 'X' && board[i][j] != 'O') {
                char temp = board[i][j];
                board[i][j] = symbol;
                if (checkWinner() == symbol) {
                    board[i][j] = temp;
                    return true;
                }
                board[i][j] = temp;
            }
        }
    }
    return false;
}

// Check if the game is effectively a draw (no winning moves possible)
bool isEffectiveDraw() {
    if (checkWinner() != ' ') return false;
    
    // If either player can win, it's not a draw yet
    if (canWin(player) || canWin(ai)) return false;
    
    // Check if there are at least 2 moves left for potential tactics
    int emptySpaces = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != 'X' && board[i][j] != 'O') {
                emptySpaces++;
            }
        }
    }
    
    // If very few spaces left and no one can win, call it a draw
    return emptySpaces <= 2;
}

// Player move with special commands
bool playerMove() {
    string input;
    cout << "Your move (1-9, + for new game, . to exit): ";
    cin >> input;
    
    if (input == "+") {
        return false; // Signal new game
    }
    if (input == ".") {
        cout << "Thanks for playing! Goodbye!\n";
        exit(0);
    }
    
    int move;
    try {
        move = stoi(input);
    } catch (...) {
        cout << "Invalid input! Use 1-9, + for new game, or . to exit.\n";
        return playerMove();
    }
    
    int row = (move - 1) / 3;
    int col = (move - 1) % 3;
    if (move < 1 || move > 9 || board[row][col] == 'X' || board[row][col] == 'O') {
        cout << "Invalid move! Try again.\n";
        return playerMove();
    } else {
        board[row][col] = player;
        return true; // Continue current game
    }
}

// AI move: win → block → random
void aiMove() {
    // Try win or block
    for (char symbol : {ai, player}) {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++) {
                if (board[i][j] != 'X' && board[i][j] != 'O') {
                    char temp = board[i][j];
                    board[i][j] = symbol;
                    if (checkWinner() == symbol) {
                        if (symbol == ai) return;  // win
                        else {
                            board[i][j] = ai; // block
                            return;
                        }
                    }
                    board[i][j] = temp;
                }
            }
    }
    // Random move
    vector<pair<int, int>> empty;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] != 'X' && board[i][j] != 'O')
                empty.push_back({i, j});
    if (!empty.empty()) {
        srand(time(0));
        pair<int, int> move = empty[rand() % empty.size()];
        board[move.first][move.second] = ai;
    }
}

// Play a single game
void playGame() {
    resetBoard();
    char winner = ' ';
    cout << "\n🎮 New Game Started! You (X) vs Computer (O)\n";
    cout << "Commands: 1-9 to play, + for new game, . to exit\n";
    
    while (true) {
        printBoard();
        
        // Check for early draw before player move
        if (isEffectiveDraw()) {
            cout << "🤝 Early draw detected - no winning moves possible!\n";
            cout << "Starting new game...\n";
            return;
        }
        
        // Player move
        if (!playerMove()) {
            cout << "🔄 Starting new game...\n";
            return; // New game requested
        }
        
        winner = checkWinner();
        if (winner == player) {
            printBoard();
            cout << "🎉 You win!\n";
            break;
        }
        
        if (isBoardFull()) {
            printBoard();
            cout << "🤝 It's a draw!\n";
            break;
        }
        
        // AI move
        aiMove();
        winner = checkWinner();
        if (winner == ai) {
            printBoard();
            cout << "🤖 Computer wins!\n";
            break;
        }
        
        if (isBoardFull()) {
            printBoard();
            cout << "🤝 It's a draw!\n";
            break;
        }
        
        // Check for early draw after AI move
        if (isEffectiveDraw()) {
            printBoard();
            cout << "🤝 Early draw detected - no winning moves possible!\n";
            cout << "Starting new game...\n";
            return;
        }
    }
}

int main() {
    cout << "🎮 Welcome to Enhanced Tic-Tac-Toe!\n";
    cout << "Commands: 1-9 to play, + for new game, . to exit\n";
    
    while (true) {
        playGame();
        
        cout << "\nPress Enter to start another game, or type . and Enter to exit: ";
        string input;
        cin.ignore();
        getline(cin, input);
        
        if (input == ".") {
            cout << "Thanks for playing! Goodbye!\n";
            break;
        }
    }
    
    return 0;
}