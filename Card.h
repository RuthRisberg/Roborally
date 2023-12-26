#ifndef Card_h
#define Card_h

#include "Action.h"

class Card {
  public:
    Card() {}
    Card(Action a, int n);
    Action getAction();
    int number;
  private:
    Action action;
};

#endif