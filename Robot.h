#ifndef Robot_h
#define Robot_h

#include "Utility.h"

class Robot {
  public:
    Robot();
    Robot(Robot&& other);
    Robot& operator=(Robot&& other);
    void setSpawn();
    int facing;
    int x;
    int y;
    int lives;
    int damage;
    int respawnx;
    int respawny;
    int currentflag;
    bool poweredDown;
    bool dead;

  private:
};

#endif