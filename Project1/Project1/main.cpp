// main.cpp

#include "Game.h"
#include <cstdlib>
#include <ctime>

///////////////////////////////////////////////////////////////////////////
//  main()
///////////////////////////////////////////////////////////////////////////

int main()
{
    // Initialize the random number generator.  (You don't need to
    // understand how this works.)
    srand(static_cast<unsigned int>(time(0)));
    
    // Create a game
    // Use this instead to create a mini-game:
    Game g(5, 5, 4);
    //Game g(9, 10, 40);
    
    // Play the game
    g.play();
}
 

