//
//  mazequeue.cpp
//  HW2
//
//  Created by Connor Kenny on 1/29/15.
//  Copyright (c) 2015 Connor Kenny. All rights reserved.
//

#include <iostream>
#include <queue>
#include <string>

using namespace std;

class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
{
    queue<Coord> q;
    
    q.push(Coord(sr, sc));
    maze[sr][sc] = '#';
    
    while(!q.empty())
    {
        Coord c = q.front();
        q.pop();
        if (c.r() == er && c.c() == ec)
            return true;
        if (maze[c.r()-1][c.c()] == '.')        // North
        {
            q.push(Coord(c.r()-1, c.c()));
            maze[c.r()-1][c.c()] = '#';
        }
        
        if (maze[c.r()][c.c()+1] == '.')        // East
        {
            q.push(Coord(c.r(), c.c()+1));
            maze[c.r()][c.c()+1] = '#';
        }
        
        if (maze[c.r()+1][c.c()] == '.')        // South
        {
            q.push(Coord(c.r()+1, c.c()));
            maze[c.r()+1][c.c()] = '#';
        }
        
        if (maze[c.r()][c.c()-1] == '.')        // West
        {
            q.push(Coord(c.r(), c.c()-1));
            maze[c.r()][c.c()-1] = '#';
        }
    }
    return false;
}
int main()
{
    string maze[10] = {
        "XXXXXXXXXX",
        "X........X",
        "XX.X.XXXXX",
        "X..X.X...X",
        "X..X...X.X",
        "XXXX.XXX.X",
        "X.X....XXX",
        "X..XX.XX.X",
        "X...X....X",
        "XXXXXXXXXX"
    };
    
    if (pathExists(maze, 10,10, 6,4, 1,1))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}
