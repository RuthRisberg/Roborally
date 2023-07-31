#ifndef Tile_h
#define Tile_h

#include "Utility.h"
#include <initializer_list>

typedef TileType::tilemodifier TM;

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
    void addtm(TileType::tilemodifier);
    Action getAction(int phase, int turn);
    
    Move::moveOutcome moveToHere(int direction);
  
  private:
    bool conveyor[4] = {false, false, false, false};
    bool expressConveyor[4] = {false, false, false, false};
    bool conveyorEntry[4] = {false, false, false, false}; // only added for curved conveyors
    bool wall[4] = {false, false, false, false};
    bool hole = false;
    bool nearhole[4] = {false, false, false, false};
    int laser = 0; //number of lasers
    bool wrench = false;
    bool wrenchAndHammer = false;
    bool pusher[4][5] = {{false,false,false,false,false},
                         {false,false,false,false,false},
                         {false,false,false,false,false},
                         {false,false,false,false,false}}; // direction, turns
    int gear = 0; //1/0/-1: clockwise/nonthing/counterclockwise
    int flag = 0;

};

#endif