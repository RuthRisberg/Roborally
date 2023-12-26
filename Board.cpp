#include "Board.h"

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))


Board::Board()
{
  for (int i = 0; i < 26; i++)
  {
    for (int j = 0; j < 26; j++)
    {
      board[i][j] = Tile({tiletype::hole, 0}, {tiletype::edge, 0});
    }
  }
  deathTile = &board[0][1];

  for (int i = 0; i < 8; i++)
  {
    robots[i] = Robot();
    robotFree[i] = true;

    revivelist[i] = &nullRobot;
  }
}

void Board::zapLasers()
{
  for (int x = 1; x < 25; x++)
  {
    for (int y = 1; y < 25; y++)
    {
      board[x][y].zapLasers();
    }
  }
}

Robot* Board::getFreeRobot()
{
  for (int i = 0; i < 8; i++)
  {
    if (robotFree[i])
    {
      robotFree[i] = false;
      return &robots[i];
    }
  }
  return &nullRobot;
}

void Board::cleanDeadRobots()
{
  for (int x = 1; x < 25; x++)
  {
    for (int y = 1; y < 25; y++)
    {
      board[x][y].removeDeadRobot();
    }
  }
}

void Board::updatePlayerTile(Player *player)
{
  player->tile = &board[player->getX()][player->getY()];
}

void Board::respawn(Robot *robot)
{
  Tile respawnTile = board[robot->respawnx][robot->respawny];
  respawnTile.reviveRobotHere(robot);
}

void Board::reviveRobots()
{
  for (int i = 0; i < 8; i++)
    {
      if (!revivelist[i]->dead)
      {
        break;
      }
      respawn(revivelist[i]);
    }
}

void Board::setBoard(int chooseboard, int place) {
  using namespace tiletype;
  // define types of boards (use the collapse thingy in the margin)
  Tile chopshop[12][12] = {
    {Tile({wrench, 0}), Tile(), Tile({wall, 0}), Tile({expressConveyor, 0}), Tile({wall, 0}), Tile({expressConveyor, 2}),
    Tile({conveyor, 0}), Tile({wall, 0}), Tile({conveyor, 2}), Tile({wall, 0}), Tile(), Tile()},

    {Tile(), Tile({hole, 0}), Tile({wall, 1}, {wall, 2}), Tile({wall, 3}, {expressConveyor, 0}), Tile(),
    Tile({wall, 1}, {expressConveyor, 2}, {laser, 3}), Tile({wall, 3}, {conveyor, 0}), Tile(), Tile({conveyor, 2}),
    Tile({conveyor, 2}, {conveyorEntry, 1}), Tile({conveyor, 3}), Tile({conveyor, 3})},

    {Tile({wall, 3}), Tile(), Tile({wall, 0}), Tile({expressConveyor, 0}, {conveyorEntry, 1}), Tile({expressConveyor, 3}),
    Tile({expressConveyor, 3}, {conveyorEntry, 0}), Tile({conveyor, 0}), Tile(), Tile({conveyor, 2}), Tile({conveyor, 2}), Tile(), Tile({wall, 1})},


    {Tile(), Tile(), Tile(), Tile({expressConveyor, 0}), Tile({wall, 1}), Tile({wall, 2}, {wall, 3}), 
    Tile({conveyor, 0}), Tile(), Tile({wall, 2}), Tile({hole, 0}), Tile(), Tile()},

    {Tile({wall, 3}), Tile(), Tile(), Tile({expressConveyor, 0}), Tile(), Tile({conveyor, 1}, {wall, 0}),
    Tile({gear, -1}), Tile(), Tile({wall, 0}), Tile({wrenchAndHammer, 0}), Tile(), Tile({wall, 1})},

    {Tile({conveyor, 3}), Tile({conveyor, 3}), Tile({gear, 1}), Tile({expressConveyor, 0}), Tile(), 
    Tile({wrenchAndHammer, 0}, {wall, 1}), Tile({conveyor, 2}, {conveyorEntry, 1}, {wall, 3}), Tile({conveyor, 3}),
    Tile({gear, 1}), Tile({conveyor, 3}), Tile({conveyor, 3}), Tile({conveyor, 3})},


    {Tile({conveyor, 1}), Tile({conveyor, 2}, {conveyorEntry, 3}), Tile({gear, -1}), Tile({expressConveyor, 0}), Tile(), Tile(),
    Tile({conveyor, 2}), Tile({hole, 0}), Tile({wall, 2}, {laser, 0}), Tile(), Tile({wall, 2}), Tile()},

    {Tile({wall, 3}), Tile({conveyor, 2}), Tile({wall, 2}, {laser, 0}), Tile({expressConveyor, 0}), Tile(), Tile(),
    Tile({conveyor, 2}), Tile(), Tile({wall, 0}), Tile(), Tile({wall, 0}), Tile({wall, 1})},

    {Tile(), Tile({conveyor, 2}), Tile({wall, 0}, {wall, 1}), Tile({wall, 3}, {laser, 2}), Tile({gear, 1}, {laser, 2}),
    Tile({gear, -1}, {laser, 2}), Tile({wall, 1}, {laser, 3}, {laser, 3}), Tile({wall, 3}), Tile({gear, -1}),
    Tile({conveyor, 3}), Tile({conveyor, 3}), Tile({conveyor, 3})},


    {Tile({wall, 3}), Tile({conveyor, 2}), Tile({wrenchAndHammer, 0}), Tile({conveyor, 0}), Tile(), Tile({hole}),
    Tile(), Tile(), Tile({conveyor, 2}), Tile({hole, 0}), Tile(), Tile({wall, 1})},

    {Tile({wall, 1}), Tile({wall, 1}, {wall, 3}, {laser, 3}, {laser, 3}, {laser, 3}), Tile({wall, 3}), Tile({conveyor, 0}), Tile(), Tile(),
    Tile({wall, 1}), Tile({wall, 3}, {laser, 1}), Tile({conveyor, 2}, {laser, 1}), Tile({wall, 1}, {laser, 1}),
    Tile({wall, 3}), Tile()},

    {Tile(), Tile({conveyor, 2}), Tile({wall, 2}), Tile({conveyor, 0}), Tile({wall, 2}), Tile(),
    Tile(), Tile({wall, 2}), Tile({conveyor, 2}), Tile({wall, 2}), Tile(), Tile({wrench, 0})}
  };

  // handle choosing what place the board is in
  int minx = 1;
  int miny = 1;
  switch (place) {
    case 0:
      break;
    case 1:
      minx += 12;
      break;
    case 2:
      miny += 12;
      break;
    case 3:
      minx += 12;
      miny += 12;
      break;
  }

  // add the board
  switch (chooseboard) {
    case 0:
      for (int i=0; i<12; i++) {
        for (int j=0; j<12; j++) {
          board[i+minx][j+miny] = chopshop[i][j];
        }
      }
      break;
  }

  // update neighbors
  for (int x = 0; x < 26; x++)
  {
    for (int y = 0; y < 26; y++)
    {
      board[x][y].addNeighbors(&board[max(0,x-1)][y], &board[x][min(25,y+1)], &board[min(25,x+1)][y], &board[x][max(0,y-1)]);
      board[x][y].setPosition(x, y);
      board[x][y].setDeathTile(deathTile);
    }
  }
}
