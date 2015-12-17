//
//  maze.cpp
//  HW3
//
//  Created by Connor Kenny on 2/5/15.
//  Copyright (c) 2015 Connor Kenny. All rights reserved.
//

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
{
    if(sr == er && sc == ec)
        return true;
    maze[sr][sc] = '#';     // '#' == visited
    
    if (maze[sr-1][sc] == '.')        // North
    {
        if (pathExists(maze, nRows, nCols, sr-1, sc, er, ec))
            return true;
    }
    
    if (maze[sr][sc+1] == '.')        // East
    {
        if (pathExists(maze, nRows, nCols, sr, sc+1, er, ec))
            return true;
    }
    
    if (maze[sr+1][sc] == '.')        // South
    {
        if (pathExists(maze, nRows, nCols, sr+1, sc, er, ec))
            return true;
    }
    
    if (maze[sr][sc-1] == '.')        // West
    {
        if (pathExists(maze, nRows, nCols, sr, sc-1, er, ec))
            return true;
    }
    
    return false;
}