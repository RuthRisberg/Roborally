#include "Card.h"

Card::Card(Action a, int n) {action = a; number = n;}

Action Card::getAction()
{
    return action;
}
