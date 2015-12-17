//
//  Pit.h
//  Project1
//
//  Created by Connor Kenny on 1/6/15.
//  Copyright (c) 2015 Connor Kenny. All rights reserved.
//

#ifndef __Project1__Pit__
#define __Project1__Pit__

#include "globals.h"
#include "History.h"
#include <string>

class Player;
class Snake;

class Pit
{
public:
    // Constructor/destructor
    Pit(int nRows, int nCols);
    ~Pit();
    
    // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     snakeCount() const;
    int     numberOfSnakesAt(int r, int c) const;
    void    display(std::string msg) const;
    History& history();
    
    // Mutators
    bool   addSnake(int r, int c);
    bool   addPlayer(int r, int c);
    bool   destroyOneSnake(int r, int c);
    bool   moveSnakes();
    
private:
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Snake*  m_snakes[MAXSNAKES];
    int     m_nSnakes;
    History m_history;
};

#endif /* defined(__Project1__Pit__) */
