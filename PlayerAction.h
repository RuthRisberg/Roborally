#ifndef PlayerAction_h
#define PlayerAction_h

#include "Player.h"
#include "Action.h"
#include "Board.h"
#include "Utility.h"

class PlayerAction
{
  public:

    PlayerAction();
    PlayerAction(Action *action, Player *player, Controls *controls, Board *board);

    bool execute(); // returns false when a move is blocked (for dead robots, use isDead())
    int xDiff();
    int yDiff();

  private:
    Controls *controls;
    Player* players[8];
    Board* board; // used to clean dead robots when needed

    Action *action; // responsible to delete
    Player *player;

    bool nothingAction = false;

    bool moveAbs(int dir, bool powermove);

    bool pushOther(int dir);

};


#endif