#include "Board.h"

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

#define ex tiletype::expressConveyor
#define co tiletype::conveyor
#define ce tiletype::conveyorEntry
#define wr tiletype::wrench
#define wh tiletype::wrenchAndHammer
#define lz tiletype::laser
#define wa tiletype::wall
#define ge tiletype::gear
#define hl tiletype::hole
#define fl tiletype::flag

Board::Board()
{
  for (int i = 0; i < boardSizeX; i++)
  {
    for (int j = 0; j < boardSizeY; j++)
    {
      board[i][j] = nullptr;
    }
  }
  for (int i = 0; i < 8; i++)
  {
    deathRow[i] = nullptr;
    robots[i] = nullptr;
    robotFree[i] = false;
    players[i] = nullptr; // needed to properly check if they're dead after lasers
  }

  controls = nullptr;
  deathTile = nullptr;
  nPlayers = 0;
}

Board::~Board()
{
  for (int i = 0; i < boardSizeX; i++)
  {
    for (int j = 0; j < boardSizeY; j++)
    {
      delete board[i][j];
    }
  }

  for (int i = 0; i < 8; i++)
  {
    delete robots[i];
  }
}

Board::Board(Board&& other)
{
  controls = other.controls;
  for (int i = 0; i < boardSizeX; i++)
  {
    for (int j = 0; j < boardSizeY; j++)
    {
      board[i][j] = other.board[i][j]; // owner
    }
  }
  deathTile = other.deathTile;
  for (int i = 0; i < 8; i++)
  {
    deathRow[i] = other.deathRow[i];
    robots[i] = other.robots[i]; // owner
    robotFree[i] = other.robotFree[i];
    players[i] = other.players[i]; // needed to properly check if they're dead after lasers
  }
  nPlayers = other.nPlayers;

  
  other.controls = nullptr;
  for (int i = 0; i < boardSizeX; i++)
  {
    for (int j = 0; j < boardSizeY; j++)
    {
      other.board[i][j] = nullptr; // owner
    }
  }
  deathTile = nullptr;
  for (int i = 0; i < 8; i++)
  {
    deathRow[i] = nullptr;
    robots[i] = nullptr; // owner
    robotFree[i] = false;
    players[i] = nullptr; // needed to properly check if they're dead after lasers
  }
  int nPlayers = 0;
}

Board& Board::operator=(Board&& other)
{
  if (this != &other)
  {    
    for (int i = 0; i < boardSizeX; i++)
    {
      for (int j = 0; j < boardSizeY; j++)
      {
        delete board[i][j];
      }
    }

    for (int i = 0; i < 8; i++)
    {
      delete robots[i];
    }



    controls = other.controls;
    for (int i = 0; i < boardSizeX; i++)
    {
      for (int j = 0; j < boardSizeY; j++)
      {
        board[i][j] = other.board[i][j]; // owner
      }
    }
    deathTile = other.deathTile;
    for (int i = 0; i < 8; i++)
    {
      deathRow[i] = other.deathRow[i];
      robots[i] = other.robots[i]; // owner
      robotFree[i] = other.robotFree[i];
      players[i] = other.players[i]; // needed to properly check if they're dead after lasers
    }
    nPlayers = other.nPlayers;

    

    other.controls = nullptr;
    for (int i = 0; i < boardSizeX; i++)
    {
      for (int j = 0; j < boardSizeY; j++)
      {
        other.board[i][j] = nullptr; // owner
      }
    }
    deathTile = nullptr;
    for (int i = 0; i < 8; i++)
    {
      deathRow[i] = nullptr;
      robots[i] = nullptr; // owner
      robotFree[i] = false;
      players[i] = nullptr; // needed to properly check if they're dead after lasers
    }
    int nPlayers = 0;
  }
  board[1][2]->printProperties();
  return *this;
}

Board::Board(Controls* controls)
{
  Serial.println("Board constructor");
  this->controls = controls;

  for (int i = 0; i < boardSizeX; i++)
  {
    for (int j = 0; j < boardSizeY; j++)
    {
      board[i][j] = new Tile({tiletype::hole, 0}, {tiletype::edge, 0});
    }
  }
  deathTile = board[12][13];
  for (int i = 0; i < 8; i++)
  {
    deathRow[i] = board[i+1][13];
  }

  Serial.println("Board constructor almost done");

  for (int i = 0; i < 8; i++)
  {
    robots[i] = new Robot();
    robotFree[i] = true;

    //revivelist[i] = &nullRobot;
  }
  Serial.println("Board constructor done");
}

void Board::zapLasers()
{
  for (int x = 1; x < 13; x++)
  {
    for (int y = 1; y < 13; y++)
    {
      board[x][y]->zapLasers();
    }
  }

  // check if anyone died from the lasers
  for (int i = 0; i < nPlayers; i++)
  {
    if (players[i]->isDead())
    {
      continue;
    } else
    {
      players[i]->checkDead();
      if (players[i]->isDead())
      {
        cleanDeadRobot(players[i]);
      }
    }
  }
}

Robot* Board::getFreeRobot(Player* player)
{
  players[nPlayers] = player;
  nPlayers += 1;
  for (int i = 0; i < 8; i++)
  {
    if (robotFree[i])
    {
      robotFree[i] = false;
      return robots[i];
    }
  }
  return nullptr;
}

void Board::cleanDeadRobot(Player* player)
{
  player->tile->hasRobot = false;
  if (controls->hasRealHoles)
  {
    //Serial.println("Death with real holes");
    controls->moveTo(player->getX(), player->getY(), false, 2*4);
    controls->moveTo(deathTile->x, deathTile->y, false, 2*4);
    controls->moveTo(deathTile->x, deathTile->y, true, 0);
  }
  else
  {
    //Serial.println("Death without real holes");
    player->tile->removeDeadRobot();
    controls->moveTo(deathTile->x, deathTile->y, true, 0);
  }

  // move the robot from the death hole to the next free death row spot
  for (int i = 0; i < 8; i++)
  {
    if (!deathRow[i]->hasRobot)
    {
      controls->moveRobotTo(player->robot, deathRow[i]->x, deathRow[i]->y, 0);
      deathRow[i]->hasRobot = true;
      deathRow[i]->robot = player->robot;
      break;
    }
  }
  updatePlayerTile(player);
}

void Board::updatePlayerTile(Player *player)
{
  player->tile->removeRobot();
  player->tile = board[player->getX()][player->getY()];
  player->tile->setRobot(player->robot);
}

void Board::respawn(Robot *robot)
{
  board[robot->respawnx][robot->respawny]->reviveRobotHere(robot);
}

void Board::reviveRobots()
{
  for (int i = 0; i < 8; i++)
  {
    if (deathRow[i]->hasRobot)
    {
      respawn(deathRow[i]->robot);
    }
  }
}

void Board::setBoard(int chooseboard, int place, bool hasRealHoles) {
  controls->hasRealHoles = hasRealHoles;

  nPlayers = 0;

  // define types of boards (use the collapse thingy in the margin)
  // Tile test[12][12] = {
  //   {Tile(), Tile(), Tile(), Tile(), Tile({fl, 1}), Tile({fl, 2}), Tile({fl,3}), Tile(), Tile({co, 1}), Tile(), Tile({ex, 1}), Tile()},

  //   {Tile(), Tile({wa, 0}), Tile(), Tile({wa, 1}), Tile(), Tile({wa, 2}), Tile(), Tile({wa, 3}), Tile(), Tile({wa, 2}), Tile(), Tile()},

  //   {Tile(), Tile(), Tile({ge, 1}), Tile(), Tile(), Tile({ge, -1}), Tile(), Tile(), Tile(), Tile({lz, 0}), Tile(), Tile()},


  //   {Tile(), Tile({ex, 2}), Tile(), Tile(), Tile({ex, 2}), Tile(), 
  //   Tile(), Tile(), Tile(), Tile(), Tile(), Tile()},

  //   {Tile({ex, 1}), Tile({ex, 1}, {ce, 2}), Tile({ex, 1}), Tile({ex, 1}), Tile({ex, 2}, {ce, 3}), Tile({ex, 3}), 
  //   Tile(), Tile(), Tile({co, 2}), Tile(), Tile({co, 2}), Tile()},

  //   {Tile(), Tile({ex, 0}), Tile(), Tile(), Tile({ex, 2}), Tile(), 
  //   Tile(), Tile({co, 1}), Tile({co, 2}, {ce, 1}), Tile({co, 3}), Tile({co, 3}, {ce, 2}), Tile({co, 3})},


  //   {Tile(), Tile({ex, 0}), Tile(), Tile(), Tile({ex, 2}), Tile(), 
  //   Tile(), Tile(), Tile({co, 2}), Tile(), Tile({co, 0}), Tile()},

  //   {Tile({ex, 1}), Tile({ex, 0}, {ce, 1}), Tile({ex, 3}), Tile({ex, 3}), Tile({ex, 3}, {ce, 0}), Tile({ex, 3}), 
  //   Tile(), Tile({co, 1}), Tile({co, 1}, {ce, 0}), Tile({co, 1}), Tile({co, 0}, {ce, 3}), Tile({co, 3})},

  //   {Tile(), Tile({ex, 0}), Tile(), Tile(), Tile({ex, 0}), Tile(), 
  //   Tile(), Tile(), Tile({co, 0}), Tile(), Tile({co, 0}), Tile()},


  //   {Tile(), Tile(), Tile(), Tile(), Tile(), Tile({lz, 2}), 
  //   Tile(), Tile({hl}), Tile(), Tile(), Tile(), Tile()},

  //   {Tile(), Tile({wa, 3}), Tile(), Tile({lz, 3}), Tile(), Tile({wa, 2}), 
  //   Tile(), Tile(), Tile({lz, 1}, {lz, 1}, {lz, 1}), Tile(), Tile({wa, 1}), Tile()},

  //   {Tile({wr}), Tile(), Tile({wh}), Tile(), Tile(), Tile(), 
  //   Tile(), Tile(), Tile(), Tile(), Tile(), Tile()}
  // };

  Tile chopshop[12][12] = {
    {Tile({wr, 0}), Tile(), Tile({wa, 0}), Tile({ex, 0}), Tile({wa, 0}), Tile({ex, 2}),
    Tile({co, 0}), Tile({wa, 0}), Tile({co, 2}), Tile({wa, 0}), Tile(), Tile()},

    {Tile(), Tile({hl, 0}), Tile({wa, 1}, {wa, 2}), Tile({wa, 3}, {ex, 0}), Tile(),
    Tile({wa, 1}, {ex, 2}, {lz, 3}), Tile({wa, 3}, {co, 0}), Tile(), Tile({co, 2}),
    Tile({co, 2}, {ce, 1}), Tile({co, 3}), Tile({co, 3})},

    {Tile({wa, 3}), Tile(), Tile({wa, 0}), Tile({ex, 0}, {ce, 1}), Tile({ex, 3}),
    Tile({ex, 3}, {ce, 0}), Tile({co, 0}), Tile(), Tile({co, 2}), Tile({co, 2}), Tile(), Tile({wa, 1})},


    {Tile(), Tile(), Tile(), Tile({ex, 0}), Tile({wa, 1}), Tile({wa, 2}, {wa, 3}), 
    Tile({co, 0}), Tile(), Tile({wa, 2}), Tile({hl, 0}), Tile(), Tile()},

    {Tile({wa, 3}), Tile(), Tile(), Tile({ex, 0}), Tile(), Tile({co, 1}, {wa, 0}),
    Tile({ge, -1}), Tile(), Tile({wa, 0}), Tile({wh, 0}), Tile(), Tile({wa, 1})},

    {Tile({co, 3}), Tile({co, 3}), Tile({ge, 1}), Tile({ex, 0}), Tile(), 
    Tile({wh, 0}, {wa, 1}), Tile({co, 2}, {ce, 1}, {wa, 3}), Tile({co, 3}),
    Tile({ge, 1}), Tile({co, 3}), Tile({co, 3}), Tile({co, 3})},


    {Tile({co, 1}), Tile({co, 2}, {ce, 3}), Tile({ge, -1}), Tile({ex, 0}), Tile(), Tile(),
    Tile({co, 2}), Tile({hl, 0}), Tile({wa, 2}, {lz, 0}), Tile(), Tile({wa, 2}), Tile()},

    {Tile({wa, 3}), Tile({co, 2}), Tile({wa, 2}, {lz, 0}), Tile({ex, 0}), Tile(), Tile(),
    Tile({co, 2}), Tile(), Tile({wa, 0}), Tile(), Tile({wa, 0}), Tile({wa, 1})},

    {Tile(), Tile({co, 2}), Tile({wa, 0}, {wa, 1}), Tile({wa, 3}, {lz, 2}), Tile({ge, 1}, {lz, 2}),
    Tile({ge, -1}, {lz, 2}), Tile({wa, 1}, {lz, 3}, {lz, 3}), Tile({wa, 3}), Tile({ge, -1}),
    Tile({co, 3}), Tile({co, 3}), Tile({co, 3})},


    {Tile({wa, 3}), Tile({co, 2}), Tile({wh, 0}), Tile({co, 0}), Tile(), Tile({hl}),
    Tile(), Tile(), Tile({co, 2}), Tile({hl, 0}), Tile(), Tile({wa, 1})},

    {Tile({wa, 1}), Tile({wa, 1}, {wa, 3}, {lz, 3}, {lz, 3}, {lz, 3}), Tile({wa, 3}), Tile({co, 0}), Tile(), Tile(),
    Tile({wa, 1}), Tile({wa, 3}, {lz, 1}), Tile({co, 2}, {lz, 1}), Tile({wa, 1}, {lz, 1}),
    Tile({wa, 3}), Tile()},

    {Tile(), Tile({co, 2}), Tile({wa, 2}), Tile({co, 0}), Tile({wa, 2}), Tile(),
    Tile(), Tile({wa, 2}), Tile({co, 2}), Tile({wa, 2}), Tile(), Tile({wr, 0})}
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
    // case -1:
    //   for (int i=0; i<12; i++) {
    //     for (int j=0; j<12; j++) {
    //       *board[i+minx][j+miny] = Tile(test[i][j]);
    //     }
    //   }
    //   break;
    case 0:
      for (int i=0; i<12; i++) {
        for (int j=0; j<12; j++) {
          *board[i+minx][j+miny] = Tile(chopshop[i][j]);
        }
      }
      break;
  }

  // update neighbors and controls
  for (int x = 0; x < boardSizeX; x++)
  {
    for (int y = 0; y < boardSizeY; y++)
    {
      Tile *n0 = board[max(0,x-1)][y];
      Tile *n1 = board[x][min(25,y+1)];
      Tile *n2 = board[min(25,x+1)][y];
      Tile *n3 = board[x][max(0,y-1)];
      if (x == 0) n0 = nullptr;
      if (y == boardSizeY-1) n1 = nullptr;
      if (x == boardSizeX-1) n2 = nullptr;
      if (y == 0) n3 = nullptr;
      board[x][y]->addNeighbors(n0, n1, n2, n3);
      board[x][y]->setPosition(x, y);
      board[x][y]->setDeathTile(deathTile);

      board[x][y]->controls = controls;
    }
  }
  board[1][2]->printProperties();
  board[2][2]->printProperties();
}
