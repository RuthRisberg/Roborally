#include "Robot.h"
#include "Controls.h"
#include "Action.h"

Robot::Robot(Robot&& other)
{
    facing=other.facing;
    x=other.x;
    y=other.y;
    lives=other.lives;
    damage=other.damage;
    respawnx=other.respawnx;
    respawny=other.respawny;
    currentflag=other.currentflag;
    poweredDown=other.poweredDown;
    dead = other.dead;
    
    facing=0;
    x=0;
    y=0;
    lives=0;
    damage=0;
    respawnx=0;
    respawny=0;
    currentflag=0;
    poweredDown=false;
    dead = false;
}

Robot& Robot::operator=(Robot&& other)
{
    if (this != &other)
    {
        facing=other.facing;
        x=other.x;
        y=other.y;
        lives=other.lives;
        damage=other.damage;
        respawnx=other.respawnx;
        respawny=other.respawny;
        currentflag=other.currentflag;
        poweredDown=other.poweredDown;
        dead = other.dead;
        
        facing=0;
        x=0;
        y=0;
        lives=0;
        damage=0;
        respawnx=0;
        respawny=0;
        currentflag=0;
        poweredDown=false;
        dead = false;
    }

    return *this;
}

Robot::Robot()
{
    facing=0;
    x=1;
    y=1;
    lives=3;
    damage=0;
    respawnx=1;
    respawny=1;
    currentflag=0;
    poweredDown=false;
    dead = false;
}

void Robot::setSpawn()
{
    respawnx = x;
    respawny = y;
}
