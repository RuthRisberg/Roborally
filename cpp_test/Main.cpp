// test a copy of roborally
#include <iostream>
#include "../Game.h"

int main()
{

    // Controls controls;
    // controls.init();
    // controls.moveTo(1,1,true,0); // negative x/negative y corner, up, facing negative x with positive y to the right
    // controls.moveTo(12,12,false,2); // opposite corner, down, rotated 180 degrees

    Game game;
    game.init();
    game.setNumberOfPlayers(1);
    game.runGame();

}