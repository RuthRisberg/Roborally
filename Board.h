#ifndef Board_h
#define Board_h

#include "Utility.h"
#include "Robot.h"
#include "Tile.h"
#include "Controls.h"
#include "Player.h"

class Board {
  public:
    Board();
    void setBoard(int board, int place);
    void zapLasers(); // tell each tile to zap lasers
    Robot* getFreeRobot();
    void cleanDeadRobots();
    void reviveRobots();
    void updatePlayerTile(Player *player);

  private:
    Tile board[26][26];
    Tile *deathTile;
    Robot robots[8];
    Robot nullRobot;
    bool robotFree[8];

    Robot* revivelist[8];

    void respawn(Robot *player);
};

#endif