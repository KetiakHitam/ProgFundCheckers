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
bool movementchoices(char input[3], int player, char** board, int size, int targets[2][2], int &targetCount);
void movePiece(char input[3], int targetRow, int targetCol, int player, char** board);

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
        int targets[2][2];
        int targetCount = 0;
        bool hasMove = movementchoices(pos, currentTurn, board, size, targets, targetCount);
        // If the selected piece has no valid moves, allow the player to choose another piece until a pice with valid moves is selected
        while (!hasMove) {
            cout << "Player " << currentTurn << ", " << "please select a piece by typing row then column. E.g A1" << endl;
            cin >> pos;
            validInput = validateInput(pos, size, currentTurn, board);

            while (validInput == false)
            {
                cout << "Invalid input, please select a piece by typing row then column. E.g A1" << endl;
                cin >> pos;
                validInput = validateInput(pos, size, currentTurn, board);
            }
            cout << "selected piece at "<< pos << endl;
                hasMove = movementchoices(pos, currentTurn, board, size, targets, targetCount);
            }


        while (hasMove) {
            cout << "Please type the target position (e.g. B3): ";
            char choice[3];
            cin >> choice;

            // Temporarily stores the targetted position's row and column
            int tcol = choice[0] - 'A';
            int trow = choice[1] - '0' - 1;
            int chosenRow = -1;
            int chosenCol = -1;
            // Checks the array targets if the targetted position matches the row and column stored
            for (int i = 0; i < targetCount; ++i) {
                if (targets[i][0] == trow && targets[i][1] == tcol) {
                    chosenRow = trow;
                    chosenCol = tcol;
                    break;
                }
            }
            // If the chosen position matches with targets available, moves the pieces and restarts the loop after displaying board
            if (chosenRow != -1) {
                movePiece(pos, chosenRow, chosenCol, currentTurn, board);
                displayBoard(board, size);
                hasMove = false; // Exit the loop after a successful move
            }
            // If the chosen position does not match with targets available, and lets the player to choose again
            else {
                cout << "Invalid choice or square not available." << endl;
            }
        }
    }

        deleteBoard(board, size);

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

// Function to handle movement choices for pieces
bool movementchoices(char input[3], int player, char** board, int size, int targets[2][2], int &targetCount)
{
    targetCount = 0;
    int col = input[0] - 'A';
    int row = input[1] - '0' - 1;
    char targetPiece = 'X';
    // Determines if the piece moves up or down depending on the current player on the turn
    int drow;
    if(player==1){
    drow=1;
    targetPiece = 'O';
    }
    else
    {
    drow=-1;
    targetPiece = 'X';
    }

    // Calculates targettable position on the left of the piece
    int t1r = row + drow;
    int t1c = col - 1;
    // Calculates targettable position on the right of the piece
    int t2r = row + drow;
    int t2c = col + 1;

    cout << "You may move this piece to the following positions:" << endl;

    // Checks if targettable positions are actually playable '.'
    if (t1r >= 0 && t1r < size && t1c >= 0 && t1c < size && board[t1r][t1c] == '.' || (board[t1r][t1c] == targetPiece)) {
        if (board[t1r][t1c] == '.')
        {
            cout << static_cast<char>('A' + t1c) << (t1r + 1) << endl;
            targets[targetCount][0] = t1r;
            targets[targetCount][1] = t1c;
            targetCount++;
        }
        else if (board[t1r][t1c] == targetPiece) // if its an enemy piece then we should check the next position
        {
            cout << "ATTACK THE D POINT (LEFT)" << endl;
            if (board[t1r + drow][--t1c] == '.') // if theres an empty and movable space to then we allow this position to be used
            {
                cout << static_cast<char>('A' - t1c * 2) << (t1r + 2) << endl;
                targets[targetCount][0] = t1r;
                targets[targetCount][1] = t1c;
                targetCount++;
            }

        }

    }


    if (t2r >= 0 && t2r < size && t2c >= 0 && t2c < size && board[t2r][t2c] == '.') {
        if (board[t2r][t2c] == '.')
        {
            cout << static_cast<char>('A' + t2c) << (t2r + 1) << endl;
            targets[targetCount][0] = t2r;
            targets[targetCount][1] = t2c;
            targetCount++;
        }
        else if (board[t1r][t1c] == targetPiece) // if its an enemy piece then we should check the next position
        {
            cout << "ATTACK THE D POINT (RIGHT)" << endl;
            if (board[t2r + drow][++t2c] == '.') // if theres an empty and movable space to then we allow this position to be used
            {
                cout << static_cast<char>('A' - t2c * 2) << (t2r - 2) << endl;
                targets[targetCount][0] = t2r;
                targets[targetCount][1] = t2c;
                targetCount++;
            }

        }

    }



    if (targetCount == 0) {
        cout << "(It seems there are no legal moves with this piece)" << endl;
    }

    return targetCount > 0;
}

// Function to handle actual movement of a piece
void movePiece(char input[3], int targetRow, int targetCol, int player, char** board)
{
    int fromCol = input[0] - 'A';
    int fromRow = input[1] - '0' - 1;

    if (targetRow < 0 || targetCol < 0) {
        cout << "No move chosen." << endl;
        return;
    }

    board[targetRow][targetCol] = board[fromRow][fromCol];
    board[fromRow][fromCol] = '.';
    cout << "Moved piece to " << static_cast<char>('A' + targetCol) << (targetRow + 1) << endl;
}

// Dynamically allocates a 2D char array using pointers.
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
