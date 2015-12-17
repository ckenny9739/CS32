//
//  History.cpp
//  Project1
//
//  Created by Connor Kenny on 1/6/15.
//  Copyright (c) 2015 Connor Kenny. All rights reserved.
//

#include "History.h"
#include <iostream>

using namespace std;

History::History(int nRows, int nCols)
{
    m_rows = nRows;
    m_cols = nCols;
    
    // Sets up inital grid of only '.'
    for (int r = 0; r < m_rows; r++)
        for (int c = 0; c < m_cols; c++)
        {
            m_hGrid[r][c] = '.';
        }
}
bool History::record(int r, int c)
{
    if (r <= m_rows || c <= m_cols)
    {
        // '.' for none, 'A' - 'Y' for 1 - 25, 'Z' for 26+
        switch (m_hGrid[r-1][c-1])
        {
            case '.': m_hGrid[r-1][c-1] = 'A'; break;
            case 'Z': break;
            default: m_hGrid[r-1][c-1]++;
        }
        return true;
    }
    return false;
}
void History::display() const
{
    clearScreen();
    
    // Print out updated grid
    for (int r = 0; r < m_rows; r++)
    {
        for (int c = 0; c < m_cols; c++)
            cout << m_hGrid[r][c];
        cout << endl;
    }
    cout << endl;
}