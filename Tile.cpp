#include "Arduino.h"
#include "Tile.h"


Tile::Tile() {}
Tile::Tile(TM t1) {addtm(t1);};
Tile::Tile(TM t1, TM t2) {addtm(t1);addtm(t2);};
Tile::Tile(TM t1, TM t2, TM t3) {addtm(t1);addtm(t2);addtm(t3);};
Tile::Tile(TM t1, TM t2, TM t3, TM t4) {addtm(t1);addtm(t2);addtm(t3);addtm(t4);};
Tile::Tile(TM t1, TM t2, TM t3, TM t4, TM t5) {addtm(t1);addtm(t2);addtm(t3);addtm(t4);addtm(t5);};
Tile::Tile(TM t1, TM t2, TM t3, TM t4, TM t5, TM t6) {addtm(t1);addtm(t2);addtm(t3);addtm(t4);addtm(t5);addtm(t6);};
Tile::Tile(TM t1, TM t2, TM t3, TM t4, TM t5, TM t6, TM t7) {addtm(t1);addtm(t2);addtm(t3);addtm(t4);addtm(t5);addtm(t6);addtm(t7);};
Tile::Tile(TM t1, TM t2, TM t3, TM t4, TM t5, TM t6, TM t7, TM t8) {addtm(t1);addtm(t2);addtm(t3);addtm(t4);addtm(t5);addtm(t6);addtm(t7);addtm(t8);};

void Tile::addtm(TileType::tilemodifier modifier) {
  switch (modifier.type) {
    case TileType::conveyor:
      conveyor[modifier.modifier] = true;

    case TileType::expressConveyor:
      expressConveyor[modifier.modifier] = true;

    case TileType::wall:
      wall[modifier.modifier] = true;
      
    case TileType::hole:
      hole = true;

    case TileType::laser:
      laser = modifier.modifier;

    case TileType::wrench:
      wrench = true;

    case TileType::wrenchAndHammer:
      wrenchAndHammer = true;

    case TileType::pusher:
      pusher[modifier.modifier/10][modifier.modifier%10] = true;

    case TileType::gear:
      gear = modifier.modifier;
      
    case TileType::flag:
      flag = modifier.modifier;

    default:
      Serial.println("Missed an enum in Tile::Tile");
  }
}

// needs support for turning conveyors
Action Tile::getAction(int phase, int turn) {
  switch(phase) {
    case 5: // express conveyor belts
      for (int i=0; i<4; i++) {
        if (expressConveyor[i]) {
          Action a = {Action::moveabs, i};
          return (a);
        }
      }
    case 6: // express and normal conveyor belts
      for (int i=0; i<4; i++) {
        if (conveyor[i] or expressConveyor[i]) {
          Action a = {Action::moveabs, i};
          return (a);
        }
      }
    case 7: // pushers
      for (int i=0; i<4; i++){
        if (pusher[i][turn]) {
          Action a = {Action::moveabs, (i+2)%4};
          return (a);
        }
      }
    case 8: // gears
      if (gear != 0) {
        Action a = {Action::rotation, gear};
        return (a);
      }
    case 9: // lasers
      if (laser != 0) {
        Action a = {Action::damage, laser};
        return (a);
      }
    case 10: // register flags
      if (flag) {
        Action a = {Action::flag, flag};
        return (a);
      }
    case 11: // save new spawnpoints
      if (wrench || flag || wrenchAndHammer) {
        Action a = {Action::setspawn, 0};
        return (a);
      }
    case 12: // repair
      if (wrench || wrenchAndHammer) {
        Action a = {Action::damage, -1};
        return (a);
      }
    default:
      Action a = {Action::nothing, 0};
      return (a);
  }
}

Move::moveOutcome Tile::moveToHere(int direction) 
{
  if (wall[direction]) return {Move::fail, -1};
  if (hole) return {Move::death, 0};
  return {Move::success, 0};
}

