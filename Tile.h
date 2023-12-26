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

    Action getAction(int phase, int turn);
    move::moveOutcome moveToHere(int headingDirection);
    move::moveOutcome moveAway(int direction);
    Tile* getNeighbor(int dir);
    void zapLasers();
    void removeDeadRobot();
    void reviveRobotHere(Robot *robot);

    void setPosition(int x, int y);
    void setDeathTile(Tile *deathTile);

    void addNeighbors(Tile *n0, Tile *n1, Tile *n2, Tile *n3);

    void printProperties(); //debug
  
  private:
    void init();
    Controls *controls;
    Robot *robot;
    bool hasRobot;
    Tile *n0;
    Tile *n1;
    Tile *n2;
    Tile *n3;
    Tile *deathTile;
    int x;
    int y;
    bool conveyor[4];
    bool expressConveyor[4];
    bool conveyorEntry[4]; // only added for curved conveyors
    bool wall[4];
    bool hole;
    bool wrench;
    bool wrenchAndHammer;
    bool pusher[4][5]; // direction, turns
    int gear; //1/0/-1: clockwise/nonthing/counterclockwise
    int flag;
    int nlasers; //number of lasers
    int laserdir;
    bool edge;


    int dfsvar;

    void addtm(tilemodifier modifier);

    void runLaser(int dir);

    void updatePathToDeath(int distance); // part of the bfs
    bool pullRobot(bool towardsDeath, int lastDfsVar, Robot *robot);
    void resetDfs();
};

#endif