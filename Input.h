#ifndef Input_h
#define Input_h

#include "Player.h"
#include "Card.h"
#include "Action.h"

class Input
{
  public:
    Input() {nPlayers = 0;}
    Input(int nPlayers, Player *players);
    void readCards();
    void readPowerDown(int phase);

  private:
    Player *players;
    int nPlayers;
};

#endif