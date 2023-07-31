#ifndef Game_h
#define Game_h

#include "Tile.h"
#include "Utility.h"

/**
Phases:
0 (deal cards)
1 (read cards)
2 announce power down
  3 reveal card
  4 move robots
  5 express conveyor belts
  6 express and normal conveyor belts
  7 pushers
  8 gears
  9 lasers
  10 register flags
  11 save new spawnpoints
12 repair
13 (draw option card)
14 (release cards)
15 announce keep being powered down
16 respawn (powered down?)
*/

class Game{
  public:
    Game();
    void setBoard(int board, int place);
    void setNumberOfPlayers(int players);
    void runGame();
    Move::moveOutcome robotAction(Action action, Robot robot);
    void die(Robot robot);
    void undo(Robot robot, Action action);
    void readcards();
    void readpowerdown();
    void respawn();
  
  private:
    Tile board[26][26];
    Robot robots[4];
    int nPlayers;
};


#endif