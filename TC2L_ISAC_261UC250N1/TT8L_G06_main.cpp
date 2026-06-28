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
bool validateInput(char input[2], int size, int player, char** board);
bool validatePiece(int column, int row, int player, char** board);
void movementchoices(char input[3], int player, char** board);
void movePiece(char input[3], char movechoices[3], int player, char** board);

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
        movementchoices(pos, currentTurn, board);
        movePiece(pos, currentTurn, board);

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
// Will fix when I figure everything else out.
void movementchoices(char input[3], int player, char** board)
{
    // Handle movement for player 1's pieces (X), top of the board
    if (player ==1)
    {
        char movechoices[3];
        if (input[0] != size -1 || input[1] != 0 || input[1] != size -1)
        {   
            cout <<"You may move this piece to the following positions: " << endl;
            cout << input[0] + 1 << input[1] - 1 << endl;
            cout << input[0] + 1 << input[1] + 1 << endl;
            movechoices[0] = input[0] + 1;
            movechoices[1] = input[1] - 1;
            movechoices[2] = input[1] + 1;
        }
        else if (input[0] != 0 || input[1] != size -1)
        {
            cout <<"You may move this piece to the following positions: " << endl;
            cout << input[0] + 1 << input[1] + 1 << endl;
            movechoices[0] = input[0] + 1;
            movechoices[1] = input[1] + 1;
        }
        else if (input[0] != 0 || input[1] != 0)
        {
            cout <<"You may move this piece to the following positions: " << endl;
            cout << input[0] + 1 << input[1] - 1 << endl;
            movechoices[0] = input[0] + 1;
            movechoices[1] = input[1] - 1;
        }
        else
        {
            cout <<"This piece is at the edge of the board and cannot move." << endl;
        }
    }
    // Handle movement for player 2's pieces (O), bottom of the board
    else if (player == 2)
    {   
        if (input[0] != 0 || input[1] != 0 || input[1] != size -1)
        {
            cout <<"You may move this piece to the following positions: " << endl;
            cout << input[0] - 1 << input[1] - 1 << endl;
            cout << input[0] - 1 << input[1] + 1 << endl;
            movechoices[0] = input[0] - 1;
            movechoices[1] = input[1] - 1;
            movechoices[2] = input[1] + 1;
        }
        else if (input[0] != 0 || input[1] != size -1)
        {
            cout <<"You may move this piece to the following positions: " << endl;
            cout << input[0] - 1 << input[1] + 1 << endl;
            movechoices[0] = input[0] - 1;
            movechoices[1] = input[1] + 1;
        }
        else if (input[0] != 0 || input[1] != 0)
        {
            cout <<"You may move this piece to the following positions: " << endl;
            cout << input[0] - 1 << input[1] - 1 << endl;
            movechoices[0] = input[0] - 1;
            movechoices[1] = input[1] - 1;
        }
        else
        {
            cout <<"This piece is at the edge of the board and cannot move." << endl;
        }
    }
}

// Function to handle actual movement of a piece
void movePiece(char input[3], char movechoices[3], int player, char** board)
{
    cout <<"Please select a position to move to from the available choices." << endl;
    cin >> movementCHOICE;
    if (movementCHOICE[0] == movechoices[0] && movementCHOICE[1] == movechoices[1])
    {
        cout <<"Moving piece to " << movementCHOICE << endl;
        board[movementCHOICE[0]][movementCHOICE[1]] = board[input[0]][input[1]];
        board[input[0]][input[1]] = '.';
    }
    else if (movementCHOICE[0] == movechoices[0] && movementCHOICE[1] == movechoices[2])
    {
        cout <<"Moving piece to " << movementCHOICE << endl;
        board[movementCHOICE[0]][movementCHOICE[1]] = board[input[0]][input[1]];
        board[input[0]][input[1]] = '.';
    }
    else
    {
        cout <<"Invalid move choice. Please select a valid position." << endl;
    }
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
