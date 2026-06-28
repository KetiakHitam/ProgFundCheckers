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
bool movementchoices(char input[3], int player, char** board, int size, int targets[2][2], int &targetCount, bool capturesOnly = false);
bool movePiece(char input[3], int targetRow, int targetCol, int player, char** board);
bool isEnemy(char piece, int player);

// Entry point: sets up or restores the game, then runs the turn loop until a win.
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

        // Piece selection. Reading into a string first lets the player type SAVE
        // (longer than the 2-char coordinate buffer) without overflowing pos.
        bool validInput = false;
        string entry;
        while (!validInput)
        {
            cout << "Player " << currentTurn << ", select a piece (e.g. A1) or type SAVE to save: " << endl;
            cin >> entry;

            // SAVE trigger. saveGame writes the file and, if the player chooses to
            // quit, exits the program; otherwise it returns and we prompt again.
            if (entry == "SAVE" || entry == "save")
            {
                saveGame(board, size, currentTurn);
                continue;
            }

            // A coordinate needs at least a column letter and a row digit.
            if (entry.length() < 2)
            {
                cout << "Invalid input." << endl;
                continue;
            }

            pos[0] = entry[0];
            pos[1] = entry[1];
            pos[2] = '\0';
            validInput = validateInput(pos, size, currentTurn, board);
        }
        cout << "selected piece at " << pos << endl;

        displayBoard(board, size);
        int targets[2][2];
        int targetCount = 0;
        bool hasMove = movementchoices(pos, currentTurn, board, size, targets, targetCount);
        // If the selected piece has no legal moves, let the player pick another
        // piece until one with a move is chosen.
        while (!hasMove) {
            cout << "Player " << currentTurn << ", select a piece (e.g. A1): " << endl;
            cin >> pos;
            validInput = validateInput(pos, size, currentTurn, board);

            while (validInput == false)
            {
                cout << "Invalid input, select a piece (e.g. A1): " << endl;
                cin >> pos;
                validInput = validateInput(pos, size, currentTurn, board);
            }
            cout << "selected piece at " << pos << endl;
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
            // If the chosen position matches an available target, move the piece.
            if (chosenRow != -1) {
                // Read the moving piece before the move to detect a Juggernaut.
                char mover = board[pos[1] - '0' - 1][pos[0] - 'A'];
                bool isJuggernaut = (mover == 'J' || mover == 'j');

                bool didCapture = movePiece(pos, chosenRow, chosenCol, currentTurn, board);
                displayBoard(board, size);

                // Track the piece at its new square for chain capture or promotion.
                pos[0] = static_cast<char>('A' + chosenCol);
                pos[1] = static_cast<char>('1' + chosenRow);
                pos[2] = '\0';

                // Juggernaut: after a kill it may keep capturing (chain kill).
                if (isJuggernaut && didCapture)
                {
                    hasMove = movementchoices(pos, currentTurn, board, size, targets, targetCount, true);
                    if (hasMove)
                        cout << "Juggernaut chain kill, pick the next capture." << endl;
                }
                else
                {
                    hasMove = false;
                }

                // Turn over: offer promotion. The board was already shown right
                // after the move, so redraw only if promotion changed the piece.
                if (!hasMove)
                {
                    char before = board[chosenRow][chosenCol];
                    checkAndAssignPower(board, chosenRow, chosenCol, currentTurn, size);
                    if (board[chosenRow][chosenCol] != before)
                        displayBoard(board, size);
                }
            }
            // Otherwise the target was not a listed option; let the player retry.
            else {
                cout << "Invalid choice or square not available." << endl;
            }
        }
    }

    // Loop ended: one side has no pieces left. Announce the winner by counting
    // what remains. The side with pieces still on the board wins.
    int p1Left = 0;
    int p2Left = 0;
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            char c = board[row][col];
            if (c == 'X' || c == 'P' || c == 'J' || c == 'N')
                p1Left++;
            else if (c == 'O' || c == 'p' || c == 'j' || c == 'n')
                p2Left++;
        }
    }

    cout << endl;
    if (p1Left == 0 && p2Left == 0)
        cout << "Game over. It is a draw." << endl;
    else if (p2Left == 0)
        cout << "Game over. Player 1 wins!" << endl;
    else
        cout << "Game over. Player 2 wins!" << endl;

    deleteBoard(board, size);

    // Keep the window open so the result stays visible when run by double-click.
    cout << endl << "Press Enter to exit...";
    cin.ignore(1000, '\n');
    cin.get();

    return 0;
}


// Prompts for the board size and re-asks until it is within MIN_SIZE..MAX_SIZE.
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

// Validates a coordinate (letter then digit, both in range) and checks the square
// holds one of the current player's pieces.
bool validateInput(char input[3], int size, int player, char** board)
{
    int colLetter = input[0] - 'A';     // 'A' maps to column 0
    int rowNumber = input[1] - '0' - 1; // '1' maps to row 0
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

// Confirms the square holds a piece (normal or powered) owned by the current player.
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

// When a normal piece reaches the opponent's back row, lets the player choose a power.
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

// Lists the legal targets for the piece at input and stores them in targets.
// capturesOnly restricts the list to jumps only (used for Juggernaut chaining).
bool movementchoices(char input[3], int player, char** board, int size, int targets[2][2], int &targetCount, bool capturesOnly)
{
    targetCount = 0;
    int col = input[0] - 'A';
    int row = input[1] - '0' - 1;
    // Player 1 moves down the board, Player 2 moves up.
    int drow;
    if (player == 1)
        drow = 1;
    else
        drow = -1;

    // Phantom moves through its own team's pieces (phases over friendlies).
    char piece = board[row][col];
    bool isPhantom = (piece == 'P' || piece == 'p');

    // Diagonal column directions: left then right.
    int dcols[2] = { -1, 1 };

    if (!capturesOnly)
        cout << "You may move this piece to the following positions:" << endl;

    // Each forward diagonal yields at most one target: a step onto the first
    // empty square (Phantom phases over friendlies to reach it), or a capture
    // over an enemy onto the empty square beyond it.
    for (int i = 0; i < 2; i++)
    {
        int dc = dcols[i];
        int k = 1;

        // Phantom skips past consecutive friendly pieces along the diagonal.
        if (isPhantom)
        {
            while (true)
            {
                int rr = row + k * drow;
                int cc = col + k * dc;
                if (rr < 0 || rr >= size || cc < 0 || cc >= size)
                    break;
                char here = board[rr][cc];
                if (here == '.' || isEnemy(here, player))
                    break; // empty square to land on, or an enemy to capture
                k++;       // friendly piece: phase through it
            }
        }

        int stepR = row + k * drow;
        int stepC = col + k * dc;
        if (stepR < 0 || stepR >= size || stepC < 0 || stepC >= size)
            continue;

        char dest = board[stepR][stepC];
        if (dest == '.')
        {
            // Plain step (suppressed during a chain capture).
            if (!capturesOnly)
            {
                cout << static_cast<char>('A' + stepC) << (stepR + 1) << endl;
                targets[targetCount][0] = stepR;
                targets[targetCount][1] = stepC;
                targetCount++;
            }
        }
        else if (isEnemy(dest, player))
        {
            // Capture: land on the empty square just beyond the enemy.
            int jumpR = stepR + drow;
            int jumpC = stepC + dc;
            if (jumpR >= 0 && jumpR < size && jumpC >= 0 && jumpC < size && board[jumpR][jumpC] == '.')
            {
                cout << static_cast<char>('A' + jumpC) << (jumpR + 1) << " (capture)" << endl;
                targets[targetCount][0] = jumpR;
                targets[targetCount][1] = jumpC;
                targetCount++;
            }
        }
    }

    if (targetCount == 0 && !capturesOnly) {
        cout << "(It seems there are no legal moves with this piece)" << endl;
    }

    return targetCount > 0;
}

// Moves a piece to the target square. Returns true if the move captured an enemy.
// The captured enemy (if any) is the square one diagonal step before the landing
// square; this holds for normal jumps and for Phantom jumps over friendlies.
bool movePiece(char input[3], int targetRow, int targetCol, int player, char** board)
{
    int fromCol = input[0] - 'A';
    int fromRow = input[1] - '0' - 1;

    if (targetRow < 0 || targetCol < 0) {
        cout << "No move chosen." << endl;
        return false;
    }

    char mover = board[fromRow][fromCol];
    bool captured = false;

    // Unit diagonal step from the source toward the landing square.
    int dr = (targetRow > fromRow) ? 1 : -1;
    int dc = (targetCol > fromCol) ? 1 : -1;
    int capRow = targetRow - dr;
    int capCol = targetCol - dc;

    if (capRow >= 0 && capCol >= 0 && isEnemy(board[capRow][capCol], player))
    {
        captured = true;
        if (mover == 'N' || mover == 'n')
        {
            // Necromancer: the captured square becomes a new normal piece of your colour.
            board[capRow][capCol] = (player == 1) ? 'X' : 'O';
            cout << "Necromancer raised a piece at " << static_cast<char>('A' + capCol) << (capRow + 1) << endl;
        }
        else
        {
            board[capRow][capCol] = '.';
            cout << "Captured piece at " << static_cast<char>('A' + capCol) << (capRow + 1) << endl;
        }
    }

    board[targetRow][targetCol] = mover;
    board[fromRow][fromCol] = '.';
    cout << "Moved piece to " << static_cast<char>('A' + targetCol) << (targetRow + 1) << endl;
    return captured;
}

// Returns true if the given piece char belongs to the opponent of player.
bool isEnemy(char piece, int player)
{
    if (player == 1)
        return (piece == 'O' || piece == 'p' || piece == 'j' || piece == 'n');
    else
        return (piece == 'X' || piece == 'P' || piece == 'J' || piece == 'N');
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

// Fills the board: X (Player 1) on the top rows, O (Player 2) on the bottom rows,
// placed only on dark squares where (row + col) is odd.
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

// Draws the board with column letters, row numbers, and grid lines.
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

// Frees every row then the row-pointer array.
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
    cout << "===== CHECKERMANIA =====" << endl;
    cout << "Please type in CONTINUE to continue the game where you left off." << endl;
    cout << "Please type in NEW GAME to start a new game." << endl;
    cout << "Your input: ";
    string choice;
    cin >> choice;
    cin.ignore(1000, '\n'); // discard the rest of the line, e.g. the "GAME" in "NEW GAME"

    if (choice != "CONTINUE")
        return NULL;

    ifstream inFile;
    inFile.open("savegame.txt");

    // Open check: no save file present.
    if (!inFile)
    {
        cout << "No save found or save corrupted. Starting new game." << endl;
        return NULL;
    }

    // Read header.
    inFile >> size;
    inFile >> currentTurn;

    // Reject corrupt or out-of-range data.
    if (inFile.fail() || size < MIN_SIZE || size > MAX_SIZE)
    {
        cout << "No save found or save corrupted. Starting new game." << endl;
        inFile.close();
        return NULL;
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
