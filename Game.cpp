#include "variant.h"
#include "Arduino.h"
#include "Game.h"
#include "Tile.h"
#include "Utility.h"
#include "Motors.h"


// apparently a bug in arduino that this isn't always included by default I think
#define min(a,b) ((a)<(b)?(a):(b))

Game::Game() {}

void Game::init()
{
  Motors motors = Motors();
  motors.init();
  for (int i = 0; i < 26; i++)
  {
    for (int j = 0; j < 26; j++)
    {
      board[i][j] = Tile({TileType::hole});
    }
  }
}

void Game::setBoard(int chooseboard, int place) {
  // define types of boards (use the collapse thingy in the margin)
  typedef TileType tt;
  Tile chopshop[12][12] = {
    {Tile({tt::wrench, 0}), Tile(), Tile({tt::wall, 0}), Tile({tt::expressConveyor, 0}), Tile({tt::wall, 0}), Tile({tt::expressConveyor, 2}),
    Tile({tt::conveyor, 0}), Tile({tt::wall, 0}), Tile({tt::conveyor, 2}), Tile({tt::wall, 0}), Tile(), Tile()},

    {Tile(), Tile({tt::hole, 0}), Tile({tt::wall, 1}, {tt::wall, 2}), Tile({tt::wall, 3}, {tt::expressConveyor, 0}), Tile(),
    Tile({tt::wall, 1}, {tt::expressConveyor, 2}, {tt::laser, 3}), Tile({tt::wall, 3}, {tt::conveyor, 0}), Tile(), Tile({tt::conveyor, 2}),
    Tile({tt::conveyor, 2}, {tt::conveyorEntry, 1}), Tile({tt::conveyor, 3}), Tile({tt::conveyor, 3})},

    {Tile({tt::wall, 3}), Tile(), Tile({tt::wall, 0}), Tile({tt::expressConveyor, 0}, {tt::conveyorEntry, 1}), Tile({tt::expressConveyor, 3}),
    Tile({tt::expressConveyor, 3}, {tt::conveyorEntry, 0}), Tile({tt::conveyor, 0}), Tile(), Tile({tt::conveyor, 2}), Tile({tt::conveyor, 2}), Tile(), Tile({tt::wall, 1})},


    {Tile(), Tile(), Tile(), Tile({tt::expressConveyor, 0}), Tile({tt::wall, 1}), Tile({tt::wall, 2}, {tt::wall, 3}), 
    Tile({tt::conveyor, 0}), Tile(), Tile({tt::wall, 2}), Tile({tt::hole, 0}), Tile(), Tile()},

    {Tile({tt::wall, 3}), Tile(), Tile(), Tile({tt::expressConveyor, 0}), Tile(), Tile({tt::conveyor, 1}, {tt::wall, 0}),
    Tile({tt::gear, -1}), Tile(), Tile({tt::wall, 0}), Tile({tt::wrenchAndHammer, 0}), Tile(), Tile({tt::wall, 1})},

    {Tile({tt::conveyor, 3}), Tile({tt::conveyor, 3}), Tile({tt::gear, 1}), Tile({tt::expressConveyor, 0}), Tile(), 
    Tile({tt::wrenchAndHammer, 0}, {tt::wall, 1}), Tile({tt::conveyor, 2}, {tt::conveyorEntry, 1}, {tt::wall, 3}), Tile({tt::conveyor, 3}),
    Tile({tt::gear, 1}), Tile({tt::conveyor, 3}), Tile({tt::conveyor, 3}), Tile({tt::conveyor, 3})},


    {Tile({tt::conveyor, 1}), Tile({tt::conveyor, 2}, {tt::conveyorEntry, 3}), Tile({tt::gear, -1}), Tile({tt::expressConveyor, 0}), Tile(), Tile(),
    Tile({tt::conveyor, 2}), Tile({tt::hole, 0}), Tile({tt::wall, 2}, {tt::laser, 0}), Tile(), Tile({tt::wall, 2}), Tile()},

    {Tile({tt::wall, 3}), Tile({tt::conveyor, 2}), Tile({tt::wall, 2}, {tt::laser, 0}), Tile({tt::expressConveyor, 0}), Tile(), Tile(),
    Tile({tt::conveyor, 2}), Tile(), Tile({tt::wall, 0}), Tile(), Tile({tt::wall, 0}), Tile({tt::wall, 1})},

    {Tile(), Tile({tt::conveyor, 2}), Tile({tt::wall, 0}, {tt::wall, 1}), Tile({tt::wall, 3}, {tt::laser, 2}), Tile({tt::gear, 1}, {tt::laser, 2}),
    Tile({tt::gear, -1}, {tt::laser, 2}), Tile({tt::wall, 1}, {tt::laser, 3}, {tt::laser, 3}), Tile({tt::wall, 3}), Tile({tt::gear, -1}),
    Tile({tt::conveyor, 3}), Tile({tt::conveyor, 3}), Tile({tt::conveyor, 3})},


    {Tile({tt::wall, 3}), Tile({tt::conveyor, 2}), Tile({tt::wrenchAndHammer, 0}), Tile({tt::conveyor, 0}), Tile(), Tile({tt::hole}),
    Tile(), Tile(), Tile({tt::conveyor, 2}), Tile({tt::hole, 0}), Tile(), Tile({tt::wall, 1})},

    {Tile({tt::wall, 1}), Tile({tt::wall, 1}, {tt::wall, 3}, {tt::laser, 3}, {tt::laser, 3}, {tt::laser, 3}), Tile({tt::wall, 3}), Tile({tt::conveyor, 0}), Tile(), Tile(),
    Tile({tt::wall, 1}), Tile({tt::wall, 3}, {tt::laser, 1}), Tile({tt::conveyor, 2}, {tt::laser, 1}), Tile({tt::wall, 1}, {tt::laser, 1}),
    Tile({tt::wall, 3}), Tile()},

    {Tile(), Tile({tt::conveyor, 2}), Tile({tt::wall, 2}), Tile({tt::conveyor, 0}), Tile({tt::wall, 2}), Tile(),
    Tile(), Tile({tt::wall, 2}), Tile({tt::conveyor, 2}), Tile({tt::wall, 2}), Tile(), Tile({tt::wrench, 0})}
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
}

void Game::setNumberOfPlayers(int players) {
  nPlayers = players;
  for (int i=0; i<min(players, 4); i++) {
    robots[i] = Robot(i);
  }
}

void Game::runGame() {
  /**
  Phases:
  0 (deal cards)
  1 (read cards)
  2 announce power down
    3 reveal card
    4 move robots
    5 express conveyor belts
    6 express and normal conveyor belts
    7 pushers
    8 gears
    9 lasers
    10 register flags
    11 save new spawnpoints
  12 repair
  13 (draw option card)
  14 (release cards)
  15 announce keep being powered down
  16 respawn (powered down?)
  */

  // setup:
  // motors.home();
  setBoard(0, 0);
  nPlayers = 1;
  for (int i = 0; i < nPlayers; i++)
  {
    revivelist[i] = -1;
  }
  robots[0].respawnx = 1;
  robots[0].respawny = 1;
  robots[0].x = 1;
  robots[0].y = 1;
  robots[0].facing = 1;

  // for each round:
  while (true)
  {
    // phase 0

    // phase 1
    readcards();

    // phase 2
    readpowerdown();

    // phases 3-11
    for (int reg = 0; reg < 5; reg++)
    {
      // phase 3


      // phase 4
      int irobots[nPlayers];

      // find player order
      for (int i = 0; i < nPlayers; i++)
      {
        irobots[i] = -1;
        for (int j = 0; j < nPlayers; j++)
        {
          if (i > 0 && robots[irobots[i-1]].cards[reg].number >= robots[j].cards[reg].number) 
          {
            continue;
          }

          if (irobots[i] == -1)
          {
            irobots[i] = j;
          }
          else if (robots[irobots[i]].cards[reg].number < robots[j].cards[reg].number)
          {
            irobots[i] = j;
          }
        }
      }

      // move robots
      for (int i = 0; i < nPlayers; i++)
      {
        Robot robot = robots[irobots[i]];
        Action action = robot.cards[reg].action;
        Serial.println("Phase 4 action " + String(action.type));
        Move::moveOutcome result = robotAction(action, i);
        if (result.result == Move::death)
        {
          die(i);
        }
        Serial.println(robot.y);
      }


      // phase 5-11
      for (int phase = 5; phase <= 11; phase++)
      {
        if (phase == 9) // lasers
        {
          for (int x = 0; x < 26; x++)
          {
            for (int y = 0; y < 26; y++)
            {
              if (board[x][y].nlasers > 0)
              {
                zaplaser(x, y, board[x][y].laserdir, board[x][y].nlasers);
              }
            }
          }
          for (int i = 0; i < nPlayers; i++)
          {
            zaplaser(robots[i].x, robots[i].y, robots[i].facing, 1);
          }
        }
        else {
          Serial.println("Phase " + String(phase));
          Action robotActions[nPlayers];
          for (int i = 0; i < nPlayers; i++)
          {
            robotActions[i] = Action(Action::nothing);
          }
          for (int iPlayer = 0; iPlayer < nPlayers; iPlayer++)
          {
            Robot robot = robots[iPlayer];
            Tile tile = board[robot.x][robot.y];
            Action action = tile.getAction(phase, reg);
            Move::moveOutcome outcome = robotAction(action, iPlayer);
            if (outcome.result == Move::success)
            {
              robotActions[iPlayer] = action;
            }
            else if (outcome.result == Move::fail && outcome.other != -1)
            {
              undo(outcome.other, robotActions[outcome.other]);
            }
            else if (outcome.result == Move::death)
            {
              die(iPlayer);
            }
          }
        }
      }
    }

    // phase 12
    for (int iPlayer = 0; iPlayer < nPlayers; iPlayer++) 
    {
      Robot robot = robots[iPlayer];
      Tile tile = board[robot.x][robot.y];
      Action action = tile.getAction(12, -1); // reg can only be relevant for phase 3-11
      robotAction(action, iPlayer);
    }

    // phase 13
    // phase 14

    // phase 15
    readpowerdown();

    // phase 16
    for (int i = 0; i < nPlayers; i++)
    {
      if (revivelist[i] == -1)
      {
        break;
      }
      respawn(revivelist[i]);
    }
  }
}

Move::moveOutcome Game::robotAction(Action action, int iPlayer) 
{
  // automatically fail if dead or powered down
  if (robots[iPlayer].damage >= 10 || robots[iPlayer].poweredDown)
  {
    return {Move::fail, -1};
  }
  // if this is a composite action, call again for each part
  if (action.secondaryActiontype != Action::nothing)
  {
    Move::moveOutcome result;
    Action primaryAction = action;
    Action secondaryAction = Action(action.secondaryActiontype, action.property21, action.property22);
    primaryAction.secondaryActiontype = Action::nothing;

    result = robotAction(primaryAction, iPlayer);
    if (result.result != Move::success)
    {
      return result;
    }
    result = robotAction(secondaryAction, iPlayer);
    if (result.result != Move::success)
    {
      undo(iPlayer, primaryAction);
    }
    return result;
  }
  switch (action.type)
  {
    case (Action::damage):
    {
      robots[iPlayer].damage += action.property1;
      if (robots[iPlayer].damage > 9) return {Move::death, 0};
      else
      {
        return {Move::success, 0};
      }
    }
    case (Action::die):
    {
      return {Move::death, 0};
    }
    case (Action::flag):
    {
      if (robots[iPlayer].currentflag + 1 == action.property1)
      {
        robots[iPlayer].currentflag++;
      }
      robots[iPlayer].respawnx = robots[iPlayer].x;
      robots[iPlayer].respawny = robots[iPlayer].y;
      return {Move::success, 0};
    }
    case (Action::moveabs):
    {
      int newx = robots[iPlayer].x;
      int newy = robots[iPlayer].y;
      switch (action.property1)
      {
        case 0:
          newx--;
          break;
        case 1:
          newy++;
          break;
        case 2:
          newx++;
          break;
        case 3:
          newy--;
          break;
      }
      Serial.println("New coordinates");
      Serial.println(newx);
      Serial.println(newy);
      Move::moveOutcome tileresult = board[newx][newy].moveToHere(action.property1);
      Serial.println("Outcome");
      Serial.println(tileresult.result);
      if (tileresult.result != Move::success)
      {
        return tileresult;
      }

      Move::moveOutcome thisresult = {Move::success, 0};
      for (int i = 0; i < nPlayers; i++)
      {
        if (robots[i].x == newx && robots[i].y == newy)
        {
          Move::moveOutcome result = robotAction(action, i);
          if (result.result == Move::fail)
          {
            return {Move::fail, i};
          }
          else {
            thisresult = {Move::push, i};
          }
        }
      }
      moveTo(robots[iPlayer].x, robots[iPlayer].y, false, robots[iPlayer].facing);
      moveTo(robots[iPlayer].x, robots[iPlayer].y, true, robots[iPlayer].facing);
      moveTo(newx, newy, true, robots[iPlayer].facing);
      moveTo(newx, newy, false, robots[iPlayer].facing);
      robots[iPlayer].x = newx;
      robots[iPlayer].y = newy;
      Serial.println(robots[iPlayer].y);
      return thisresult;
    }
    case (Action::moverel): // calls robotAction for moveabs
    {
      for (int i = 0; i < action.property2; i++)
      {
        Serial.println("Call moveabs");
        Serial.println(action.property2);
        Move::moveOutcome result = robotAction(Action(Action::moveabs, (action.property1 + robots[iPlayer].facing)%4), iPlayer);
        if (result.result != Move::success)
        {
          return result;
        }
      }
      return {Move::success, 0};
    }
    case (Action::nothing):
    {
      return {Move::success, 0};
    }
    case (Action::rotation):
    {
      moveTo(robots[iPlayer].x, robots[iPlayer].y, false, robots[iPlayer].facing);
      moveTo(robots[iPlayer].x, robots[iPlayer].y, true, robots[iPlayer].facing);
      robots[iPlayer].facing += action.property1;
      moveTo(robots[iPlayer].x, robots[iPlayer].y, true, robots[iPlayer].facing);
      moveTo(robots[iPlayer].x, robots[iPlayer].y, false, robots[iPlayer].facing);
      robots[iPlayer].facing %= 4;
      return {Move::success, 0};
    }
    case (Action::setspawn):
    {
      robots[iPlayer].respawnx = robots[iPlayer].x;
      robots[iPlayer].respawny = robots[iPlayer].y;
      return {Move::success, 0};
    }
    case (Action::upgrade):
    {
      return {Move::success, 0};
    }
  }
}

void Game::die(int iPlayer)
{
  robots[iPlayer].x = 0;
  robots[iPlayer].y = 0;
  robots[iPlayer].facing = 0; // off the field, to make sure it doesn't shoot anything
  for (int i = 0; i < nPlayers; i++)
  {
    if (revivelist[i] == -1)
    {
      revivelist[i] = iPlayer;
      break;
    }
  }
}

void Game::undo(int iPlayer, Action action)
{

}

void Game::readcards()
{
  for (int iPlayer = 0; iPlayer < nPlayers; iPlayer++)
  {
    Serial.println("Player " + String(iPlayer) + "'s turn to pick cards!");
    for (int iCard = 0; iCard < min(5, 9-robots[iPlayer].damage); iCard++)
    {
      // 0 - back 1
      // 1-3 - forward X
      // 4 - left turn
      // 5 - right turn
      // 6 - u-turn
      Serial.println("Card number " + String(iCard) + ":");
      while (Serial.available() == 0);
      int cardtype = Serial.readStringUntil(' ').toInt();
      int cardnumber = Serial.readStringUntil('\n').toInt();
      switch (cardtype) {
        case 0:
          robots[iPlayer].cards[iCard] = Card(Action(Action::moverel, 2, 1), cardnumber);
          Serial.println("back 1");
          break;
        case 1:
          robots[iPlayer].cards[iCard] = Card(Action(Action::moverel, 0, 1), cardnumber);
          Serial.println("forward 1");
          break;
        case 2:
          robots[iPlayer].cards[iCard] = Card(Action(Action::moverel, 0, 2), cardnumber);
          Serial.println("forward 2");
          break;
        case 3:
          robots[iPlayer].cards[iCard] = Card(Action(Action::moverel, 0, 3), cardnumber);
          Serial.println("forward 3");
          break;
        case 4:
          robots[iPlayer].cards[iCard] = Card(Action(Action::rotation, -1), cardnumber);
          Serial.println("rotate left");
          break;
        case 5:
          robots[iPlayer].cards[iCard] = Card(Action(Action::rotation, 1), cardnumber);
          Serial.println("rotate right");
          break;
        case 6:
          robots[iPlayer].cards[iCard] = Card(Action(Action::rotation, 2), cardnumber);
          Serial.println("u-turn");
          break;
      }
    }
  }
}

void Game::readpowerdown()
{

}

void Game::respawn(int iPlayer)
{

}

void Game::moveTo(int x, int y, bool up, int angle)
{
  Serial.println("moveto " + String(x) + " " + String(y) + " " + String(up) + " " + String(angle));
  while (Serial.available() == 0);
  Serial.readStringUntil('\n');
}

void Game::zaplaser(int x, int y, int dir, int power)
{
  for (int i = 0; i < nPlayers; i++)
  {
    if (robots[i].x == x && robots[i].y == y)
    {
      robots[i].damage += power;
      if (robots[i].damage >= 10)
      {
        die(i);
      }
      return;
    }
  }

  // will only get here if no robot was on this square
  int newx = x;
  int newy = y;
  switch (dir)
  {
    case 0:
      newx--;
      break;
    case 1:
      newy++;
      break;
    case 2:
      newx++;
      break;
    case 3:
      newy--;
      break;
  }
  if (board[newx][newy].moveToHere(dir).result != Move::fail)
  {
    zaplaser(newx, newy, dir, power);
  }
}




