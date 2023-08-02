#include "Utility.h"

Action::Action(){}
Action::Action(actiontype at){type=at;}
Action::Action(actiontype at, int v1){type=at; property1=v1;}
Action::Action(actiontype at, int v1, int v2){type=at; property1=v1; property2=v2;}

Card::Card(){}
Card::Card(Action a, int n) {action = a; number = n;}

Robot::Robot(){}
Robot::Robot(int _id){id=_id;}