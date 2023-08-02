#include "RingBuffer.h"
#ifndef Game_h
#define Game_h

#include "Tile.h"
#include "Utility.h"
#include "Motors.h"

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
    void init();
    void setBoard(int board, int place);
    void setNumberOfPlayers(int players);
    void runGame();
    Move::moveOutcome robotAction(Action action, int iPlayer);
    void die(int iPlayer);
    void undo(int iPlayer, Action action);
    void readcards();
    void readpowerdown();
    void respawn(int iPlayer);
    void moveTo(int x, int y, bool up, int angle); // board tile, up/down and angle as 0-3
    void zaplaser(int x, int y, int dir, int power); // recursive

  
  private:
    Tile board[26][26];
    Robot robots[8];
    Motors motors;
    int nPlayers;
    float xpos(int x);
    float xref[4] = {0,0,0,0};
    float ypos(int y);
    float yref[4] = {0,0,0,0};
    float uppos = 0;
    float downpos = 0;
    float anglemult = 100;
    int revivelist[8];
};


#endif