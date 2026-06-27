// *********************************************************
// Program: TT8L_G06_main.cpp
// Course: CCP6114 Programming Fundamentals
// Lecture Class: TC2L
// Tutorial Class: TT8L
// Trimester: 2610
// Member Information:
// 261UC250N1 | ISAC BIN MEGAT AZLAN | ISAC.MEGAT.AZLAN1@student.mmu.edu.my | 0198333630
// 261UC250KC | Lim Lun Sheng | lim.lun.sheng1@student.mmu.edu.my | 0138340747
// 261UC250N5 | Lum Jun Keat | LUM.JUN.KEAT1@student.mmu.edu.my | 0103593989
// 261UC250XG | Hor Jin Han | Hor.Jin.Han1@student.mmu.edu.my | 0126836216
// *********************************************************

#include <iostream>
#include <iomanip>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <string>
using namespace std;

const int MIN_SIZE = 6;
const int MAX_SIZE = 10;

int getBoardSize();
char** createBoard(int size);
void initBoard(char** board, int size);
void displayBoard(char** board, int size);
void deleteBoard(char** board, int size);
bool validateInput(char input[3], int size, int player, char** board);
bool validatePiece(int column, int row, int player, char** board);

void checkAndAssignPower(char** board, int row, int col, int player, int size);
void saveGame(char** board, int size, int currentTurn);
char** loadGame(int& size, int& currentTurn, bool& continueChosen);
bool winningCondition(char** board, int size);

int main()
{
    char pos[3];
    cout << "=== CHECKERS BOARD DISPLAY ===" << endl;
    cout << endl;

    int currentTurn = 2;
    int size = 0;

    // Startup menu: CONTINUE restores the saved game, anything else starts new.
    // loadGame returns the restored board on CONTINUE, or nullptr otherwise.
    bool continueChosen = false;
    char** board = loadGame(size, currentTurn, continueChosen);

    if (continueChosen)
    {
        // The loop toggles currentTurn at the top before the move, so invert the
        // loaded value to restore the saved player's turn on resume.
        if (currentTurn == 1)
            currentTurn = 2;
        else
            currentTurn = 1;
    }
    else
    {
        // New game: ask for board size, then build and fill the board.
        size = getBoardSize();
        board = createBoard(size);
        initBoard(board, size);
        currentTurn = 2; // toggle makes Player 1 move first
    }

    cout << endl;
    displayBoard(board, size);

    // Loop until one side has no pieces left (winningCondition scans the board).
    while (!winningCondition(board, size))
    {
        if (currentTurn == 1)
        {
            currentTurn = 2;
        }
        else
        {
            currentTurn = 1;
        }
        
        bool validInput = false;
        cout << "Player " << currentTurn << ", please select a piece by typing column then row. E.g A1" << endl;
        cin >> pos;
        validInput = validateInput(pos, size, currentTurn, board);

        while (validInput == false)
        {
            cout << "Invalid input, please select a piece by typing column then row. E.g A1" << endl;
            cin >> pos;
            validInput = validateInput(pos, size, currentTurn, board);
        }
        cout << "selected piece at " << pos << endl;
        
        // Updated to match the new naming convention from your snippet
        int colLetter = pos[0] - 'A';
        int rowNumber = pos[1] - '0' - 1;
        
        // Passed rowNumber first, then colLetter, to match board[row][col] mapping
        checkAndAssignPower(board, rowNumber, colLetter, currentTurn, size);
        displayBoard(board, size);
    }

    deleteBoard(board, size);
    return 0;
}

int getBoardSize()
{
    int size;
    cout << "Enter board size (" << MIN_SIZE << "-" << MAX_SIZE << "): ";
    cin >> size;

    while (cin.fail() || size < MIN_SIZE || size > MAX_SIZE)
    {
        if (cin.eof()) break;
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input. Enter a value between "
             << MIN_SIZE << " and " << MAX_SIZE << ": ";
        cin >> size;
    }

    return size;
}

// Replaced with your updated function
bool validateInput(char input[3], int size, int player, char** board)
{
    int colLetter = input[0] - 'A'; // Use ASCII to do 0 = 48
    int rowNumber = input[1] - '0' - 1;
    if (isalpha(input[0]) and isdigit(input[1]))
    {
        if (rowNumber < size and rowNumber >= 0 and colLetter < size and colLetter >= 0)
        {
            cout << endl;
            return validatePiece(colLetter, rowNumber, player, board);
        }
    }

    return false;
}

bool validatePiece(int column, int row, int player, char** board)
{
    char current = board[row][column];
    if (player == 1)
    {
        if (current == 'X' || current == 'P' || current == 'J' || current == 'N')
        {
            cout << "Piece " << current << " selected at " << static_cast<char>('A' + column) << row + 1 << endl << endl;
            return true;
        }
        else
        {
            cout << "Your piece is not in " << static_cast<char>('A' + column) << row + 1 << endl << endl;
        }
    }
    else if (player == 2)
    {
        if (current == 'O' || current == 'p' || current == 'j' || current == 'n')
        {
            cout << "Piece " << current << " selected at " << static_cast<char>('A' + column) << row + 1 << endl << endl;
            return true;
        }
        else
        {
            cout << "Your piece is not in " << static_cast<char>('A' + column) << row + 1 << endl << endl;
        }
    }
    return false;
}

void checkAndAssignPower(char** board, int row, int col, int player, int size)
{
    int enemyRow = (player == 1) ? (size - 1) : 0;
    char currentPiece = board[row][col];

    if (row == enemyRow && (currentPiece == 'X' || currentPiece == 'O'))
    {
        cout << "=== POWER UP ELIGIBLE ===" << endl;
        cout << "Choose 1 of 3 powers:" << endl;
        cout << "1. The Phantom (P)" << endl;
        cout << "2. The Juggernaut (J)" << endl;
        cout << "3. The Necromancer (N)" << endl;
        cout << "Enter your choice (1-3): ";
        
        int choice;
        cin >> choice;
        while (cin.fail() || choice < 1 || choice > 3)
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid choice. Enter 1, 2, or 3: ";
            cin >> choice;
        }

        if (player == 1)
        {
            if (choice == 1) board[row][col] = 'P';
            else if (choice == 2) board[row][col] = 'J';
            else if (choice == 3) board[row][col] = 'N';
        }
        else 
        {
            if (choice == 1) board[row][col] = 'p';
            else if (choice == 2) board[row][col] = 'j';
            else if (choice == 3) board[row][col] = 'n';
        }
        cout << "Power assigned successfully!" << endl << endl;
    }
}

char** createBoard(int size)
{
    char** board = new char*[size];
    for (int i = 0; i < size; i++)
    {
        board[i] = new char[size];
    }
    return board;
}

void initBoard(char** board, int size)
{
    int pieceRows = (size / 2) - 1;

    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            if ((row + col) % 2 == 1) 
            {
                if (row < pieceRows)
                {
                    board[row][col] = 'X';
                }
                else if (row >= size - pieceRows)
                {
                    board[row][col] = 'O';
                }
                else
                    board[row][col] = '.';
            }
            else
            {
                board[row][col] = ' '; 
            }
        }
    }
}

void displayBoard(char** board, int size)
{
    cout << "   ";
    for (int col = 0; col < size; col++)
    {
        cout << "  " << static_cast<char>('A' + col) << " ";
    }
    cout << endl;

    for (int row = 0; row < size; row++)
    {
        cout << "   ";
        for (int col = 0; col < size; col++)
            cout << "+---";
        cout << "+" << endl;

        cout << setw(2) << (row + 1) << " ";
        for (int col = 0; col < size; col++)
        {
            cout << "| " << board[row][col] << " ";
        }
        cout << "|" << endl;
    }

    cout << "   ";
    for (int col = 0; col < size; col++)
        cout << "+---";
    cout << "+" << endl;
}

void deleteBoard(char** board, int size)
{
    for (int i = 0; i < size; i++)
    {
        delete[] board[i];
    }
    delete[] board;
}

// Saves the current game state to savegame.txt as plain text.
// Format: line 1 = size, line 2 = currentTurn, then size lines of the board grid.
// Powered pieces are stored as their own chars (P/J/N, p/j/n), so writing the
// grid also saves every power.
void saveGame(char** board, int size, int currentTurn)
{
    ofstream outFile;
    outFile.open("savegame.txt");

    // Open check. If the file could not be opened, keep playing.
    if (!outFile)
    {
        cout << "Could not save game." << endl;
        return;
    }

    // Header: board size and whose turn it is.
    outFile << size << endl;
    outFile << currentTurn << endl;

    // Board grid, one row per line.
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
            outFile << board[row][col];
        outFile << endl;
    }

    outFile.close();

    // Post-save choice: quit or continue.
    cout << "Game saved. Type Q to quit or any other key to continue: ";
    char option;
    cin >> option;

    // Quit: free the board, then end the program. exit() is from <cstdlib>.
    if (option == 'Q' || option == 'q')
    {
        deleteBoard(board, size);
        exit(0);
    }
}

// Asks CONTINUE vs NEW GAME, then restores the state saved by saveGame.
// On NEW GAME, missing file, or corrupt data: returns nullptr so the caller
// runs the normal new-game setup. On success: allocates the board, fills size
// and currentTurn through the reference parameters, sets continueChosen = true,
// and returns the board pointer.
char** loadGame(int& size, int& currentTurn, bool& continueChosen)
{
    continueChosen = false;

    // "NEW GAME" only needs its first token; anything not CONTINUE = new game.
    cout << "Type CONTINUE to resume or NEW GAME to start over: ";
    string choice;
    cin >> choice;
    cin.ignore(1000, '\n'); // discard the rest of the line, e.g. the "GAME" in "NEW GAME"

    if (choice != "CONTINUE")
        return nullptr;

    ifstream inFile;
    inFile.open("savegame.txt");

    // Open check: no save file present.
    if (!inFile)
    {
        cout << "No save found or save corrupted. Starting new game." << endl;
        return nullptr;
    }

    // Read header.
    inFile >> size;
    inFile >> currentTurn;

    // Reject corrupt or out-of-range data.
    if (inFile.fail() || size < MIN_SIZE || size > MAX_SIZE)
    {
        cout << "No save found or save corrupted. Starting new game." << endl;
        inFile.close();
        return nullptr;
    }

    // Skip the newline left after the last >> so get() starts on row 0.
    inFile.ignore();

    char** board = createBoard(size);

    // Read the grid one char at a time. get() keeps spaces; >> would skip them.
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
            inFile.get(board[row][col]);
        inFile.ignore(); // skip the end-of-line newline
    }

    inFile.close();
    continueChosen = true;
    return board;
}

// Scans the board and returns true when either side has zero pieces.
// Counts powered pieces too: P/J/N belong to player 1, p/j/n to player 2.
bool winningCondition(char** board, int size)
{
    int p1Count = 0;
    int p2Count = 0;

    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            char c = board[row][col];
            if (c == 'X' || c == 'P' || c == 'J' || c == 'N')
                p1Count++;
            else if (c == 'O' || c == 'p' || c == 'j' || c == 'n')
                p2Count++;
        }
    }

    if (p1Count == 0 || p2Count == 0)
        return true;

    return false;
}