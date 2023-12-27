#ifndef Tile_h
#define Tile_h

#include "Utility.h"
#include "Robot.h"
#include "Action.h"
#include <initializer_list>
#include "Controls.h"

typedef tilemodifier TM;

class Tile {
  public:
    Tile();
    Tile(TM t1);
    Tile(TM t1, TM t2);
    Tile(TM t1, TM t2, TM t3);
    Tile(TM t1, TM t2, TM t3, TM t4);
    Tile(TM t1, TM t2, TM t3, TM t4, TM t5);
    Tile(TM t1, TM t2, TM t3, TM t4, TM t5, TM t6);
    Tile(TM t1, TM t2, TM t3, TM t4, TM t5, TM t6, TM t7);
    Tile(TM t1, TM t2, TM t3, TM t4, TM t5, TM t6, TM t7, TM t8);

    Action* getAction(int8_t phase, int8_t turn); // remember to deallocate!
    move::moveOutcome moveAway(int8_t direction);
    Tile* getNeighbor(int8_t dir);
    void zapLasers();
    void removeDeadRobot();
    void reviveRobotHere(Robot *robot);

    void setPosition(int8_t x, int8_t y);
    void setDeathTile(Tile *deathTile);

    void addNeighbors(Tile *n0, Tile *n1, Tile *n2, Tile *n3);

    void printProperties(); //debug

    void setRobot(Robot *robot);
    void removeRobot();

    Robot *robot;
    bool hasRobot;
    int8_t x;
    int8_t y;
    
    Controls *controls;
  
  private:
    void init();
    Tile *n0;
    Tile *n1;
    Tile *n2;
    Tile *n3;
    Tile *deathTile;
    bool conveyor[4];
    bool expressConveyor[4];
    bool conveyorEntry[4]; // only added for curved conveyors
    bool wall[4];
    bool hole;
    bool wrench;
    bool wrenchAndHammer;
    //bool pusher[4][5]; // direction, turns
    int8_t gear; //1/0/-1: clockwise/nonthing/counterclockwise
    int8_t flag;
    int8_t nlasers; //number of lasers
    int8_t laserdir;
    bool edge;


    int8_t dfsvar;

    void addtm(tilemodifier modifier);

    void runLaser(int8_t dir);

    move::moveOutcome moveToHere(int8_t headingDirection); // should not be called by anyone except moveAway(int)

    void updatePathToDeath(int8_t distance); // part of the bfs
    bool pullRobot(bool towardsDeath, int8_t lastDfsVar, Robot *robot);
    void resetDfs();
};

#endif