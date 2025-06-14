#include <iostream>
#include <iomanip>
#include <random>
#include <ctime>
using namespace std;

char FrontEnd[100][100];
char BackEnd[100][100];

const int Size = 9;

// Implement HighScore feature through file handling
//  Make first move better, so that a group of numbers are available
//  Flag Limit


void FillArray();  // tested
void PrintTable(); // tested
bool RowCheck(int row);
bool ColCheck(int col);
int BombCount();
int FlagCount();
bool BombCheck(int row, int col);
void Space();
bool isSpace(int row, int col);
void GeneratingMines(); // tested
int CountBomb(int row, int col);
void Assign();
bool isFlag(int row, int col);
bool AnyMinesLeft();
void TakeInput(int IntialBombCount);
void PlayMineCraft();

int main()
{
    PlayMineCraft();
}
void PlayMineCraft()
{
    FillArray();
    PrintTable();
    GeneratingMines();
    Assign();
    int IntialBombCount = BombCount();
    TakeInput(IntialBombCount);
}

void TakeInput(int IntialBombCount)
{
    int row, col;
    bool GameOver = 0, isZero = 1;
    char Decision;
    // Game over to stop the loop if
    while (!GameOver && AnyMinesLeft())
    {
        if (FlagCount() < 10)
        {
            cout << "Do you want to flag or not ? (Y/N): ";
            cin >> Decision;
            if (Decision == 'Y' || Decision == 'y' || Decision == 'N' || Decision == 'n')
            {
                if (Decision == 'Y' || Decision == 'y')
                {
                    cout << "Enter Coordinates (row col): ";
                    cin >> row >> col;
                    if (isSpace(row, col) && isZero)
                    { // will show all the spaces if 0 clicked
                        Space();
                        isZero = 0; // only once though
                        PrintTable();
                    }
                    else
                    {
                        if (RowCheck(row) && ColCheck(col) && !isFlag(row, col) && !isSpace(row, col))
                        {
                            FrontEnd[row][col] = 'F'; // to show
                            BackEnd[row][col] = 'F';  // So no bomb there either, to actually remove the bomb
                            PrintTable();
                        }
                        else
                        {
                            cout << "Invalid Input" << endl; // if row, col is out of array, or its a flag or space than invalid input
                            // ofce the first space will just open all the other spaced but that part of the code will only work
                            //  for the first space
                        }
                    }
                }
                else
                { // if its not a flag
                    cout << "Enter Coordinates (row col): ";
                    cin >> row >> col;
                    if (isSpace(row, col) && isZero == 1)
                    {
                        Space();
                        isZero = 0;
                        PrintTable();
                    }
                    else
                    {
                        if (RowCheck(row) && ColCheck(col) && !isFlag(row, col) && !isSpace(row, col))
                        {
                            if (!BombCheck(row, col))
                            {
                                FrontEnd[row][col] = BackEnd[row][col];
                                PrintTable();
                            }
                            else
                            { // if its a bomb, didnt need this for flag
                                cout << "You Loose, Game Over" << endl;
                                cout << "Your Score is " << abs(IntialBombCount - BombCount()) << endl;
                                GameOver = 1;
                            }
                        }
                        else
                        {
                            cout << "Invalid Input" << endl; // part of the choosing no flag if, wrong coordnates
                        }
                    }
                }
            }
            else
            {
                cout << "Invalid Input" << endl; // part of the choosing yes/no if, wrong decision
            }
        }
        else
        { // part of the flag count if, if flag > 10
            if (AnyMinesLeft())
            { // mines are left, you loose, else you win
                cout << "You Loose, Game Over" << endl;
            }
            else
            {
                cout << "You Won" << endl;
            }
            cout << "Your Score is " << abs(IntialBombCount - BombCount()) << endl;
            GameOver = 1;
        }
    }
}
void FillArray()
{   // Will Basically Fill the array for first time use, can also use
    // loops to fill the array again and again, kind of like reset
    for (int row = 0; row < Size; row++)
    {
        for (int col = 0; col < Size; col++)
        {
            FrontEnd[row][col] = '.';
        }
    }
}

void PrintTable()
{
    // Will print the table, imp since want to show the table at diff times
    cout << "Flag Count: " << FlagCount() << endl; // will show flag
    cout << "   ";

    for (int col = 0; col < Size; col++)
        cout << setw(2) << col << " ";
    cout << endl;
    // setw and other spaces for formatting
    for (int row = 0; row < Size; row++)
    {
        cout << setw(2) << row << " ";
        for (int col = 0; col < Size; col++)
        {
            cout << setw(2) << FrontEnd[row][col] << " ";
        }
        cout << endl;
    }
}

void GeneratingMines()
{
    int index = 1, count = 0, filled = 0, Row[Size * Size], Col[Size * Size];

    mt19937 rng(std::time(0));
    uniform_int_distribution<int> dist(0, Size - 1);
    // Up code is to generate unique random numbers
    while (filled < 11)
    { // Filled < 11, to not complete code until ten mines are obtained

        int row = dist(rng); // Unique row
        int col = dist(rng); // Unque col
        // Code to ensure row is unque by checking with all previous rows
        for (int i = 0; i < index; i++)
        {
            if (row == Row[i])
            {
                count++;
            }
        }
        if (count < 1)
            Row[index] = row;
        // Code to ensure column is unque by checking with all previous colums
        for (int i = 0; i < index; i++)
        {
            if (col == Col[i])
            {
                count++;
            }
        }
        if (count < 1)
            Col[index] = col;
        // If they are unique, add bomb there
        if (count < 2)
        {
            if (filled < 11)
                BackEnd[row][col] = '*';
            filled++;
        }
        index++;
        count = 0;
    }
}

bool RowCheck(int row)
{ // To check row doesnt exceed size or are less than size

    if (row >= Size || row < 0)
        return 0;

    else
        return 1;
}

bool ColCheck(int col)
{ // To check col doesnt exceed size or are less than size
    if (col >= Size || col < 0)
        return 0;

    else
        return 1;
}

bool BombCheck(int row, int col)
{ // To check if bomb is there

    if (RowCheck(row) && ColCheck(col))
    { // To check row and col dont exceed size or are less than size

        if (BackEnd[row][col] == '*')
            return 1;
    }

    return 0;
}
int BombCount()
{ // Counts the bombs at particular time
    int count = 0;
    for (int row = 0; row < Size; row++)
    {
        for (int col = 0; col < Size; col++)
        {
            if (BackEnd[row][col] == '*')
                count++;
        }
    }
    return count;
}

int FlagCount()
{ // Counts the number of flags at particular time
    int count = 0;
    for (int row = 0; row < Size; row++)
    {
        for (int col = 0; col < Size; col++)
        {
            if (BackEnd[row][col] == 'F')
                count++;
        }
    }
    return count;
}

void Space()
{ // Will replace all the 0s in the backend array with spaces in the frontend array
    for (int row = 0; row < Size; row++)
    {
        for (int col = 0; col < Size; col++)
        {
            if (BackEnd[row][col] == '0')
                FrontEnd[row][col] = ' ';
        }
    }
}

bool isSpace(int row, int col)
{ // To check if it is space or not
    if (BackEnd[row][col] == '0')
        return 1;
    return 0;
}

bool isFlag(int row, int col)
{ // To check if flag is there

    if (BackEnd[row][col] == 'F')
        return 1;
    return 0;
}
bool AnyMinesLeft()
{ // To check if mines are left so to end the code or not

    int count = 0;
    for (int row = 0; row < Size; row++)
    {
        for (int col = 0; col < Size; col++)
        {
            if (BackEnd[row][col] == '*')
                count++;
        }
    }
    if (count > 0)
        return 1;
    return 0;
}

int CountBomb(int row, int col)
{ // will count bombs around a position
    int count = 0;

    if (RowCheck(row + 1))
    {

        if (BackEnd[row + 1][col] == '*')
            count++;

        if (ColCheck(col + 1))
        {
            if (BackEnd[row + 1][col + 1] == '*')
                count++;
        }

        if (ColCheck(col - 1))
        {
            if (BackEnd[row + 1][col - 1] == '*')
                count++;
        }
    }

    if (RowCheck(row - 1))
    {

        if (BackEnd[row - 1][col] == '*')
            count++;

        if (ColCheck(col + 1))
        {
            if (BackEnd[row - 1][col + 1] == '*')
                count++;
        }

        if (ColCheck(col - 1))
        {
            if (BackEnd[row - 1][col - 1] == '*')
                count++;
        }
    }

    if (ColCheck(col + 1))
        if (BackEnd[row][col + 1] == '*')
            count++;

    if (ColCheck(col - 1))
        if (BackEnd[row][col - 1] == '*')
            count++;

    return count;
}

void Assign()
{ // will assign that position to the number, works in tandem with count bomb
    for (int row = 0; row < Size; row++)
        for (int col = 0; col < Size; col++)
            if (!BombCheck(row, col))
                BackEnd[row][col] = '0' + CountBomb(row, col); // '0' to convert to char
}