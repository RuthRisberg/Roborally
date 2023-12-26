#ifndef Player_h
#define Player_h

#include "Controls.h"
#include "Tile.h"
#include "Utility.h"
#include "Action.h"
#include "Card.h"
#include "Robot.h"

class Player {
  public:
    Player() {}
    Player(Robot *robot, Controls *controls);
    Action getCardAction(int registry);
    int getCardPriority(int registry);
    Action getTileAction(int phase, int registry);
    void fetchNewCards() {}

    //bool tryMoveDir(int absoluteDir, bool withForce); // uses board's orientation, not robot's
    void moveInDirection(int dir); // calls Controls::moveRobot and updates tile
    int getX();
    int getY();
    bool isDead();
    void checkDead();
    void setCard(int index, Card card);

    Tile *tile;
    Robot *robot;
    bool powerDownNextRound = false;

  private:
    Controls *controls;
    Card cards[5];
};

#endif