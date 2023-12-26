#ifndef Robot_h
#define Robot_h

#include "Utility.h"

class Robot {
  public:
    Robot();
    void setSpawn();
    int facing=0;
    int x=0;
    int y=0;
    int lives=3;
    int damage=0;
    int respawnx=0;
    int respawny=0;
    int currentflag=0;
    bool poweredDown=false;
    bool dead = false;

  private:
};

#endif