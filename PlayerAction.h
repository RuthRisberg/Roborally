#ifndef PlayerAction_h
#define PlayerAction_h

#include "Player.h"
#include "Action.h"
#include "Utility.h"

class PlayerAction
{
  public:

    PlayerAction();
    PlayerAction(Action action, Player *player, Controls *controls);

    bool execute(); // returns false when a move is blocked (for dead robots, use isDead())
    int xDiff();
    int yDiff();

  private:
    Controls *controls;
    Player* players[8];

    Action action;
    Player *player;

    bool nothingAction = false;

    bool moveAbs(int dir, bool powermove);

    bool pushOther(int dir);

};


#endif