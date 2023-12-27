#ifndef Game_h
#define Game_h

//#include "RingBuffer.h"
#include "Utility.h"
#include "Board.h"
#include "Controls.h"
#include "Player.h"
#include "PlayerAction.h"
#include "Input.h"


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

class Game {
  public:
    Game();
    ~Game();
    void init();
    void setNumberOfPlayers(int players);
    void runGame();

    Controls controls;
    Player* players[8]; //owner
    Board* board;
    Input input;
    int nPlayers;
    void playCards(int reg);
    Tile defaultTile;

  private:
};

#endif