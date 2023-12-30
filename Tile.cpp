#include "Tile.h"

Tile::Tile() {init();}
Tile::Tile(TM t1) {init();addtm(t1);};
Tile::Tile(TM t1, TM t2) {init();addtm(t1);addtm(t2);};
Tile::Tile(TM t1, TM t2, TM t3) {init();addtm(t1);addtm(t2);addtm(t3);};
Tile::Tile(TM t1, TM t2, TM t3, TM t4) {init();addtm(t1);addtm(t2);addtm(t3);addtm(t4);};
Tile::Tile(TM t1, TM t2, TM t3, TM t4, TM t5) {init();addtm(t1);addtm(t2);addtm(t3);addtm(t4);addtm(t5);};
Tile::Tile(TM t1, TM t2, TM t3, TM t4, TM t5, TM t6) {init();addtm(t1);addtm(t2);addtm(t3);addtm(t4);addtm(t5);addtm(t6);};
Tile::Tile(TM t1, TM t2, TM t3, TM t4, TM t5, TM t6, TM t7) {init();addtm(t1);addtm(t2);addtm(t3);addtm(t4);addtm(t5);addtm(t6);addtm(t7);};
Tile::Tile(TM t1, TM t2, TM t3, TM t4, TM t5, TM t6, TM t7, TM t8) {init();addtm(t1);addtm(t2);addtm(t3);addtm(t4);addtm(t5);addtm(t6);addtm(t7);addtm(t8);};



void Tile::init()
{
  for (int8_t i = 0; i < 4; i++)
  {
    conveyor[i] = false;
    expressConveyor[i] = false;
    conveyorEntry[i] = false; // only added for curved conveyors
    wall[i] = false;
    // for (int8_t j = 0; j < 5; j++)
    // {
    //   pusher[i][j] = false;
    // }
  }
  wrench = false;
  wrenchAndHammer = false;
  hole = false;
  gear = 0; //1/0/-1: clockwise/nonthing/counterclockwise
  flag = 0;
  nlasers = 0; //number of lasers
  laserdir = 0;
  edge = false;
  hasRobot = false;
  dfsvar = 120;

  n0 = nullptr;
  n1 = nullptr;
  n2 = nullptr;
  n3 = nullptr;
  deathTile = nullptr;

  robot = nullptr;
  hasRobot = false;
  x = 0;
  y = 0;
  
  controls = nullptr;
}

void Tile::addtm(tilemodifier modifier) {
  switch (modifier.type) {
    case tiletype::conveyor:
      conveyor[modifier.modifier] = true;
      break;

    case tiletype::expressConveyor:
      expressConveyor[modifier.modifier] = true;
      break;
    
    case tiletype::conveyorEntry:
      conveyorEntry[modifier.modifier] = true;
      break;

    case tiletype::wall:
      wall[modifier.modifier] = true;
      break;
      
    case tiletype::hole:
      hole = true;
      break;

    case tiletype::laser:
      nlasers++;
      laserdir = modifier.modifier;
      break;

    case tiletype::wrench:
      wrench = true;
      break;

    case tiletype::wrenchAndHammer:
      wrenchAndHammer = true;
      break;

    // case tiletype::pusher:
    //   pusher[modifier.modifier/10][modifier.modifier%10] = true;
    //   break;

    case tiletype::gear:
      gear = modifier.modifier;
      break;
      
    case tiletype::flag:
      flag = modifier.modifier;
      break;

    case tiletype::edge:
      edge = true;
      break;
    
    default:
      Serial.println("Missed an enum in Tile::Tile");
  }
}

// needs support for turning conveyors
Action* Tile::getAction(int8_t phase, int8_t turn) {
  switch(phase) {
    case 5: // express conveyor belts
      for (int8_t i=0; i<4; i++) {
        if (expressConveyor[i]) {

          if (getNeighbor(i)->conveyorEntry[(i+2)%4]) {
            int8_t rotation = -1;
            if (getNeighbor(i)->conveyor[(i+1)%4] || getNeighbor(i)->expressConveyor[(i+1)%4]) {
              rotation = 1;
            }
            Action* comp = new CompositeAction(new AbsoluteMoveAction(i, false), new RotateAction(rotation));
            return comp;
          }
          else {
            return new AbsoluteMoveAction(i, false);
          }
        }
      }
      break;
    case 6: // express and normal conveyor belts
      for (int8_t i=0; i<4; i++) {
        if (conveyor[i] or expressConveyor[i]) {
          if (getNeighbor(i)->conveyorEntry[(i+2)%4]) {
            int8_t rotation = -1;
            if (getNeighbor(i)->conveyor[(i+1)%4] || getNeighbor(i)->expressConveyor[(i+1)%4]) {
              rotation = 1;
            }
            Action* comp = new CompositeAction(new AbsoluteMoveAction(i, false), new RotateAction(rotation));
            return comp;
          }
          else {
            return new AbsoluteMoveAction(i, false);
          }
        }
      }
      break;
    // case 7: // pushers
    //   for (int8_t i=0; i<4; i++){
    //     if (pusher[i][turn]) {
    //       Action* a = new AbsoluteMoveAction((i+2)%4, true);
    //       return (a);
    //     }
    //   }
    //   break;
    case 8: // gears
      {if (gear != 0) {
        Action* a = new RotateAction(gear);
        return (a);
      }}
      break;
    case 10: // register flags
      {if (flag) {
        Action* a = new FlagAction(flag);
        return (a);
      }}
      break;
    case 11: // save new spawnpoints
      {if (wrench || flag || wrenchAndHammer) {
        Action* a = new SaveAction();
        return (a);
      }}
      break;
    case 12: // repair
      {if (wrench || wrenchAndHammer) {
        Action* a = new DamageAction(-1);
        return (a);
      }}
      break;
  }
  Action* a = new NothingAction();
  return (a);
}

move::moveOutcome Tile::moveToHere(int8_t headingDirection) 
{
  if (wall[(headingDirection+2)%4]) return move::fail;
  if (getNeighbor((headingDirection+2)%4)->wall[headingDirection]) return move::fail;
  if (hole) return move::death;
  if (edge) return move::edge;
  if (hasRobot) return move::push;
  return move::success;
}

move::moveOutcome Tile::moveAway(int8_t direction)
{
  return getNeighbor(direction)->moveToHere(direction);
}

Tile* Tile::getNeighbor(int8_t dir)
{
  switch(dir) {
    case 0:
      return n0;
    case 1:
      return n1;
    case 2:
      return n2;
    case 3:
      return n3;
  }
  return n0;
}

void Tile::zapLasers()
{
  for (int8_t i = 0; i < nlasers; i++)
  {
    if (hasRobot)
    {
      robot->damage += 1;
    }
    else
    {
      getNeighbor(laserdir)->runLaser(laserdir);
    }
  }

  if (hasRobot)
  {
    int f = robot->facing;
    Tile* n = getNeighbor(f);
    getNeighbor(robot->facing)->runLaser(robot->facing);
  }
}

void Tile::removeDeadRobot()
{
  updatePathToDeath(0);
  deathTile->pullRobot(true, -1, robot);
  resetDfs();
}

void Tile::reviveRobotHere(Robot *robot, Tile* fromTile)
{
  dfsvar = 0;
  updatePathToDeath(0);
  Serial.print("Robot respawn: ");
  Serial.print(x);
  Serial.print(", ");
  Serial.println(y);
  fromTile->pullRobot(false, -1, robot);
  resetDfs();
}

void Tile::setPosition(int8_t x, int8_t y)
{
  this->x = x;
  this->y = y;
}

void Tile::setDeathTile(Tile *deathTile)
{
  this->deathTile = deathTile;
}

void Tile::addNeighbors(Tile *n0, Tile *n1, Tile *n2, Tile *n3)
{
  this->n0 = n0;
  this->n1 = n1;
  this->n2 = n2;
  this->n3 = n3;
}

void Tile::runLaser(int8_t dir)
{
  if (edge)
  {
    return;
  }


  switch (moveToHere(dir))
  {
    case move::fail:
      return;
    case move::push:
      robot->damage += 1;
    default:
      getNeighbor(dir)->runLaser(dir);
  }
}

void Tile::updatePathToDeath(int8_t distance)
{
  if (dfsvar <= distance || hasRobot || dfsvar > 100)
  {
    return;
  }
  // Serial.print("Update path, tile x: ");
  // Serial.print(x);
  // Serial.print(", y: ");
  // Serial.print(y);
  // Serial.print(", distance: ");
  // Serial.println(distance);

  dfsvar = distance;

  if (edge)
  {
    return;
  }
  for (int8_t i = 0; i < 4; i++)
  {
    getNeighbor(i)->updatePathToDeath(distance+1);
  }
}

bool Tile::pullRobot(bool towardsDeath, int8_t lastDfsVar, Robot *robot)
{
  if (lastDfsVar == -1 || (lastDfsVar == dfsvar + 1 && !edge))
  {
    // Serial.print("Pull robot, tile x: ");
    // Serial.print(x);
    // Serial.print(", y: ");
    // Serial.print(y);
    // Serial.print(", lastDfsVar: ");
    // Serial.print(lastDfsVar);
    // Serial.print(", dfsVar: ");
    // Serial.println(dfsvar);

    if (!towardsDeath && hasRobot) return true; // don't go all the way back if the rest is blocked. stop at what most likely is the square next to the respawn point

    if (!towardsDeath) 
    {
      Serial.print("move to ");
      Serial.print(x);
      Serial.print(", ");
      Serial.println(y);
      controls->moveRobotTo(robot, x, y, -1);
    }

    for (int8_t i = 0; i < 4; i++)
    {
      bool tookThisPath = getNeighbor(i)->pullRobot(towardsDeath, dfsvar, robot);
      if (tookThisPath) break;
    }

    if (towardsDeath) controls->moveRobotTo(robot, x, y, -1);

    return true;
  }
  else
  {
    return false;
  }
}

void Tile::printProperties()
{
  Serial.println(x);
  Serial.println(y);
  Serial.print("conveyor");
  for (int8_t i = 0; i < 4; i++) Serial.print(conveyor[i]);
  Serial.print("\nexpress");
  for (int8_t i = 0; i < 4; i++) Serial.print(expressConveyor[i]);
  Serial.print("\nentry");
  for (int8_t i = 0; i < 4; i++) Serial.print(conveyorEntry[i]);
  Serial.print("\nwall");
  for (int8_t i = 0; i < 4; i++) Serial.print(wall[i]);
  Serial.print("\npush");
  for (int8_t i = 0; i < 4; i++) for (int8_t j = 0; j < 5; j++) Serial.print(0);  // pusher[i][j]);
  Serial.print("\nother");
  Serial.print(hole);
  Serial.print(wrench);
  Serial.print(wrenchAndHammer);
  Serial.print(gear);
  Serial.print(flag);
  Serial.print(nlasers);
  Serial.print(laserdir);
  Serial.print(hasRobot);
  Serial.println(edge);
}

void Tile::resetDfs()
{
  if (dfsvar == 120)
  {
    return;
  }

  dfsvar = 120;

  if (edge)
  {
    return;
  }

  for (int8_t i = 0; i < 4; i++)
  {
    getNeighbor(i)->resetDfs();
  }
}

void Tile::setRobot(Robot *robot)
{
  hasRobot = true;
  this->robot = robot;
}

void Tile::removeRobot()
{
  hasRobot = false;
}
