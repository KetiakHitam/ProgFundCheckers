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
using namespace std;

// temp values
int player1Amount = 0;
int player2Amount = 1;


// Board size constraints
const int MIN_SIZE = 6;
const int MAX_SIZE = 10;

// Function prototypes
int getBoardSize();


char** createBoard(int size);

void initBoard(char** board, int size);
void displayBoard(char** board, int size);
void deleteBoard(char** board, int size);
bool validateInput(char input[3], int size, int player, char** board);
bool validatePiece(int column, int row, int player, char** board);
bool movementchoices(char input[3], int player, char** board, int size, int targets[2][2], int &targetCount);
void movePiece(char input[3], int targetRow, int targetCol, int player, char** board);

int main()
{
    char pos[3];
    cout << "=== CHECKERS BOARD DISPLAY ===" << endl;
    cout << endl;

    int currentTurn = 2;
    int size = getBoardSize();
    cout << endl;

    // Allocate, initialize, and display the board
    char** board = createBoard(size);
    initBoard(board, size);
    displayBoard(board, size);

    while (player1Amount > 0 or player2Amount > 0)
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
        int targets[2][2];
        int targetCount = 0;
        bool hasMove = movementchoices(pos, currentTurn, board, size, targets, targetCount);
        if (!hasMove) {
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

            int tcol = choice[0] - 'A';
            int trow = choice[1] - '0' - 1;
            int chosenRow = -1;
            int chosenCol = -1;
            for (int i = 0; i < targetCount; ++i) {
                if (targets[i][0] == trow && targets[i][1] == tcol) {
                    chosenRow = trow;
                    chosenCol = tcol;
                    break;
                }
            }

            if (chosenRow != -1) {
                movePiece(pos, chosenRow, chosenCol, currentTurn, board);
                displayBoard(board, size);
                hasMove = false; // Exit the loop after a successful move
            } 
            else {
                cout << "Invalid choice or square not available." << endl;
            }
        }

    }


    return 0;
}

// Prompts player for board size and validates input is within range.
int getBoardSize()
{
    int size;
    cout << "Enter board size (" << MIN_SIZE << "-" << MAX_SIZE << "): ";
    cin >> size;

    // Reject invalid input and re-prompt
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
    if (player == 1)
    {
        if (board[row][column] == 'X')
        {
            cout << "Piece X " << "selected at " << row << " " << column << endl << endl;
            return true;
        }
        else
        {
            cout << "Piece X is not in " << row << " " << column << endl << endl;
        }
    }
    else if (player == 2)
    {
        if (board[row][column] == 'O')
        {
            cout << "Piece O " << "selected at " << row << " " << column << endl << endl;
            return true;
        }
        else
        {
            cout << "Piece O is not in " << row << " " << column << endl << endl;
        }
    }
    return false;
}

// Function to handle movement choices for pieces
bool movementchoices(char input[3], int player, char** board, int size, int targets[2][2], int &targetCount)
{
    targetCount = 0;
    int col = input[0] - 'A';
    int row = input[1] - '0' - 1;
    // Determines if the piece moves up or down depending on the current player on the turn
    int drow;
    if(player==1){
    drow=1;
    }
    else
    {
    drow=-1;
    }

    // Calculates targettable position on the left of the piece
    int t1r = row + drow;
    int t1c = col - 1;
    // Calculates targettable position on the right of the piece
    int t2r = row + drow;
    int t2c = col + 1;

    cout << "You may move this piece to the following positions:" << endl;

    // Checks if targettable positions are actually playable '.'
    if (t1r >= 0 && t1r < size && t1c >= 0 && t1c < size && board[t1r][t1c] == '.') {
        cout << static_cast<char>('A' + t1c) << (t1r + 1) << endl;
        targets[targetCount][0] = t1r;
        targets[targetCount][1] = t1c;
        targetCount++;
    }

    if (t2r >= 0 && t2r < size && t2c >= 0 && t2c < size && board[t2r][t2c] == '.') {
        cout << static_cast<char>('A' + t2c) << (t2r + 1) << endl;
        targets[targetCount][0] = t2r;
        targets[targetCount][1] = t2c;
        targetCount++;
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

// Places pieces on the board. X = Player 1 (top), O = Player 2 (bottom).
// Pieces only go on dark squares where (row + col) is odd.
void initBoard(char** board, int size)
{
    // Each player occupies (size/2 - 1) rows of pieces
    int pieceRows = (size / 2) - 1;

    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            if ((row + col) % 2 == 1) // Dark square (playable)
            {
                if (row < pieceRows)
                {
                    board[row][col] = 'X'; // X piece
                    player1Amount++;
                }
                else if (row >= size - pieceRows)
                {

                    board[row][col] = 'O'; // O piece
                    player2Amount++;
                }
                else
                    board[row][col] = '.'; // Empty playable square

            }
            else
            {
                board[row][col] = ' '; // Light square (non-playable)
            }
        }
    }
}

// Renders the board with column letters, row numbers, and grid lines.
void displayBoard(char** board, int size)
{
    // Column headers
    cout << "   ";
    for (int col = 0; col < size; col++)
    {
        cout << "  " << static_cast<char>('A' + col) << " ";
    }
    cout << endl;

    // Rows with borders
    for (int row = 0; row < size; row++)
    {
        // Horizontal separator
        cout << "   ";
        for (int col = 0; col < size; col++)
            cout << "+---";
        cout << "+" << endl;

        // Row number and cell contents
        cout << setw(2) << (row + 1) << " ";
        for (int col = 0; col < size; col++)
        {
            cout << "| " << board[row][col] << " ";
        }
        cout << "|" << endl;
    }

    // Bottom border
    cout << "   ";
    for (int col = 0; col < size; col++)
        cout << "+---";
    cout << "+" << endl;
}

// Frees all dynamically allocated memory for the board.
void deleteBoard(char** board, int size)
{
    for (int i = 0; i < size; i++)
    {
        delete[] board[i];
    }
    delete[] board;
}
