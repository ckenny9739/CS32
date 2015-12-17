//
//  globals.h
//  Project1
//
//  Created by Connor Kenny on 1/6/15.
//  Copyright (c) 2015 Connor Kenny. All rights reserved.
//

#ifndef __Project1__utilities__
#define __Project1__utilities__

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;             // max number of rows in the pit
const int MAXCOLS = 40;             // max number of columns in the pit
const int MAXSNAKES = 180;          // max number of snakes allowed

const int UP    = 0;
const int DOWN  = 1;
const int LEFT  = 2;
const int RIGHT = 3;

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir);
bool directionToDeltas(int dir, int& rowDelta, int& colDelta);
void clearScreen();


#endif /* defined(__Project1__utilities__) */
