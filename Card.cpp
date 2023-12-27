#include "Card.h"

Card::Card(Action* a, int n) {action = a; number = n;}

Card::~Card()
{
}

Action* Card::getAction()
{
    return action;
}
