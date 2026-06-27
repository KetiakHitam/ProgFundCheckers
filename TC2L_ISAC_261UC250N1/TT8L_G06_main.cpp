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

int player1Amount = 0;
int player2Amount = 0;

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

int main()
{
    char pos[3];
    cout << "=== CHECKERS BOARD DISPLAY ===" << endl;
    cout << endl;

    int currentTurn = 2;
    int size = getBoardSize();
    cout << endl;

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
                    player1Amount++;

                }
                else if (row >= size - pieceRows)
                {
                    board[row][col] = 'O'; 
                    player2Amount++;

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