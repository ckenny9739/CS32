//
//  History.h
//  Project1
//
//  Created by Connor Kenny on 1/6/15.
//  Copyright (c) 2015 Connor Kenny. All rights reserved.
//

#ifndef __Project1__History__
#define __Project1__History__

#include "globals.h"

class History
{
public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
private:
    int m_cols;
    int m_rows;
    char m_hGrid[MAXROWS][MAXCOLS];
};


#endif /* defined(__Project1__History__) */
