#include "Solver.h"

// -------------------------------------------------
// Prüfen, ob Zahl eingesetzt werden darf  (KI Beitrag)
// -------------------------------------------------
bool isValid(int grid[], int pos, int num)
{
    int row = pos / 9;
    int col = pos % 9;

    // Zeile
    for (int c = 0; c < 9; c++)
    {
        if (grid[row * 9 + c] == num)
            return false;
    }

    // Spalte
    for (int r = 0; r < 9; r++)
    {
        if (grid[r * 9 + col] == num)
            return false;
    }

    // 3x3 Block
    int br = (row / 3) * 3;
    int bc = (col / 3) * 3;

    for (int r = 0; r < 3; r++)
    {
        for (int c = 0; c < 3; c++)
        {
            if (grid[(br + r) * 9 + bc + c] == num)
                return false;
        }
    }

    return true;
}

// -------------------------------------------------
// Erste freie Zelle suchen  (KI Beitrag)
// -------------------------------------------------
int findEmpty(int grid[])
{
    for (int i = 0; i < 81; i++)
    {
        if (grid[i] == 0)
            return i;
    }

    return -1;
}

// -------------------------------------------------
// Rekursiv Lösungen zählen  (KI Beitrag)
// -------------------------------------------------
void solveCount(int grid[], int &count)
{
    // Mehr als eine Lösung interessiert nicht
    if (count >= 2)
        return;

    int pos = findEmpty(grid);

    // Keine freie Zelle mehr -> Lösung gefunden
    if (pos == -1)
    {
        count++;
        return;
    }

    // Zahlen ausprobieren
    for (int num = 1; num <= 9; num++)
    {
        if (isValid(grid, pos, num))
        {
            grid[pos] = num;

            solveCount(grid, count);

            grid[pos] = 0;
        }
    }
}

// -------------------------------------------------
// Öffentliche Funktion  (KI Beitrag)
// -------------------------------------------------
bool checkUnique(int grid[])
{
    int work[81];

    memcpy(work, grid, sizeof(work));

    int count = 0;

    solveCount(work, count);

    return (count == 1);
}