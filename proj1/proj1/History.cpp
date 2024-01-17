#include "History.h"
#include "globals.h"
#include <iostream>

using namespace std;

History::History(int nRows, int nCols)
:m_row(nRows), m_col(nCols)
{
    // initialize grid with .
    for(int r = 0; r < m_row; r++)
    {
        for(int c = 0; c < m_col; c++)
        {
            m_grid[r][c] = '.';
        }
    }
}

bool History::record(int r, int c)
{
    if(r > m_row || c > m_col || r < 1 || c < 1)
        return false;
    if(m_grid[r-1][c-1] == '.')
        m_grid[r-1][c-1] = 'A';
    else if(m_grid[r-1][c-1] >= 'A' && m_grid[r-1][c-1] <= 'Y')
        m_grid[r-1][c-1]++;
    return true;
}

void History::display() const
{
    clearScreen();
    for (int r = 0; r < m_row; r++)
    {
        for (int c = 0; c < m_col; c++)
        {
            cout << m_grid[r][c];
        }
        cout << endl;
    }
    cout << endl;
}

