#ifndef Board_h
#define Board_h

#include "Utility.h"
#include "Robot.h"
#include "Tile.h"
#include "Controls.h"
#include "Player.h"

#define boardSizeX 14
#define boardSizeY 15

class Board {
  public:
    Board();
    ~Board();
    Board(Board&& other);
    Board& operator=(Board&& other);
    Board(Controls* controls);
    void setBoard(int board, int place, bool hasRealHoles);
    void zapLasers(); // tell each tile to zap lasers
    Robot* getFreeRobot(Player* player);
    void cleanDeadRobot(Player* player);
    void reviveRobots();
    void updatePlayerTile(Player *player);
    Robot* robots[8]; // owner

  private:
    Controls* controls;
    Tile* board[boardSizeX][boardSizeY]; // owner
    Tile *deathTile;
    Tile *deathRow[8];
    bool robotFree[8];
    Player* players[8]; // needed to properly check if they're dead after lasers
    int nPlayers = 0;

    //Player* revivelist[8];

    void respawn(Robot *robot);
};

#endif