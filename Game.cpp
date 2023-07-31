#include "Arduino.h"
#include "Game.h"

// apparently a bug in arduino that this isn't always included by default I think
#define min(a,b) ((a)<(b)?(a):(b))

Game::Game() {
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

    {Tile(), Tile({tt::hole, 0}), Tile({tt::wall, 1}, {tt::wall, 2}), Tile({tt::wall, 3}, {tt::expressConveyor, 0}, {tt::laser, 1}), Tile({tt::laser, 1}),
    Tile({tt::wall, 1}, {tt::expressConveyor, 2}, {tt::laser, 1}), Tile({tt::wall, 3}, {tt::conveyor, 0}), Tile(), Tile({tt::conveyor, 2}),
    Tile({tt::conveyor, 2}, {tt::conveyorEntry, 1}), Tile({tt::conveyor, 3}), Tile({tt::conveyor, 3})},

    {Tile({tt::wall, 3}), Tile(), Tile({tt::wall, 0}, {tt::laser}), Tile({tt::expressConveyor, 0}, {tt::conveyorEntry, 1}), Tile({tt::expressConveyor, 3}),
    Tile({tt::expressConveyor, 3}, {tt::conveyorEntry, 0}), Tile({tt::conveyor, 0}), Tile(), Tile({tt::conveyor, 2}), Tile({tt::conveyor, 2}), Tile(), Tile({tt::wall, 1})},


    {Tile(), Tile(), Tile({tt::laser}), Tile({tt::expressConveyor, 0}), Tile({tt::wall, 1}), Tile({tt::wall, 2}, {tt::wall, 3}), 
    Tile({tt::conveyor, 0}), Tile(), Tile({tt::wall, 2}), Tile({tt::hole, 0}), Tile(), Tile()},

    {Tile({tt::wall, 3}), Tile(), Tile({tt::laser, 1}), Tile({tt::expressConveyor, 0}), Tile(), Tile({tt::conveyor, 1}, {tt::wall, 0}),
    Tile({tt::gear, -1}), Tile(), Tile({tt::wall, 0}, {tt::laser, 1}), Tile({tt::wrenchAndHammer, 0}), Tile(), Tile({tt::wall, 1})},

    {Tile({tt::conveyor, 3}), Tile({tt::conveyor, 3}), Tile({tt::gear, 1}, {tt::laser, 1}), Tile({tt::expressConveyor, 0}), Tile(), 
    Tile({tt::wrenchAndHammer, 0}, {tt::wall, 1}), Tile({tt::conveyor, 2}, {tt::conveyorEntry, 1}, {tt::wall, 3}), Tile({tt::conveyor, 3}),
    Tile({tt::gear, 1}, {tt::laser, 1}), Tile({tt::conveyor, 3}), Tile({tt::conveyor, 3}), Tile({tt::conveyor, 3})},


    {Tile({tt::conveyor, 1}), Tile({tt::conveyor, 2}, {tt::conveyorEntry, 3}), Tile({tt::gear, -1}, {tt::laser, 1}), Tile({tt::expressConveyor, 0}), Tile(), Tile(),
    Tile({tt::conveyor, 2}), Tile({tt::hole, 0}), Tile({tt::wall, 2}, {tt::laser, 1}), Tile(), Tile({tt::wall, 2}), Tile()},

    {Tile({tt::wall, 3}), Tile({tt::conveyor, 2}), Tile({tt::wall, 2}, {tt::laser, 1}), Tile({tt::expressConveyor, 0}), Tile(), Tile(),
    Tile({tt::conveyor, 2}), Tile(), Tile({tt::wall, 0}), Tile(), Tile({tt::wall, 0}), Tile({tt::wall, 1})},

    {Tile(), Tile({tt::conveyor, 2}), Tile({tt::wall, 0}, {tt::wall, 1}), Tile({tt::wall, 3}, {tt::laser, 2}), Tile({tt::gear, 1}, {tt::laser, 2}),
    Tile({tt::gear, -1}, {tt::laser, 2}), Tile({tt::wall, 1}, {tt::laser, 2}), Tile({tt::wall, 3}), Tile({tt::gear, -1}),
    Tile({tt::conveyor, 3}), Tile({tt::conveyor, 3}), Tile({tt::conveyor, 3})},


    {Tile({tt::wall, 3}), Tile({tt::conveyor, 2}), Tile({tt::wrenchAndHammer, 0}), Tile({tt::conveyor, 0}), Tile(), Tile({tt::hole}),
    Tile(), Tile(), Tile({tt::conveyor, 2}), Tile({tt::hole, 0}), Tile(), Tile({tt::wall, 1})},

    {Tile({tt::wall, 1}), Tile({tt::wall, 1}, {tt::wall, 3}, {tt::laser, 3}), Tile({tt::wall, 3}), Tile({tt::conveyor, 0}), Tile(), Tile(),
    Tile({tt::wall, 1}), Tile({tt::wall, 3}, {tt::laser, 1}), Tile({tt::conveyor, 2}, {tt::laser, 1}), Tile({tt::wall, 1}, {tt::laser, 1}),
    Tile({tt::wall, 3}), Tile()},

    {Tile(), Tile({tt::conveyor, 2}), Tile({tt::wall, 2}), Tile({tt::conveyor, 0}), Tile({tt::wall, 2}), Tile(),
    Tile(), Tile({tt::wall, 2}), Tile({tt::conveyor, 2}), Tile({tt::wall, 2}), Tile(), Tile({tt::wrench, 0})}
  };

  // handle choosing what place the board is in
  int minx = 0;
  int miny = 0;
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


  // for each round:
  while (true)
  {
    // phase 0
    // phase 1

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
        Move::moveOutcome result = robotAction(action, robot);
        if (result.result == Move::death)
        {
          die(robot);
        }
      }


      // phase 5-11
      for (int phase = 5; phase <= 11; phase++)
      {
        Action robotActions[nPlayers];
        for (int i = 0; i < nPlayers; i++)
        {
          robotActions[i] = Action(Action::nothing);
        }
        for (int iPlayer = 0; iPlayer < nPlayers; iPlayer)
        {
          Robot robot = robots[iPlayer];
          Tile tile = board[robot.x][robot.y];
          Action action = tile.getAction(phase, reg);
          Move::moveOutcome outcome = robotAction(action, robot);
          if (outcome.result == Move::success)
          {
            robotActions[iPlayer] = action;
          }
          else if (outcome.result == Move::fail && outcome.other != -1)
          {
            undo(robots[outcome.other], robotActions[outcome.other]);
          }
          else if (outcome.result == Move::death)
          {
            die(robot);
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
      robotAction(action, robot);
    }

    // phase 13
    // phase 14

    // phase 15
    readpowerdown();

    // phase 16
    respawn();
  }
}

Move::moveOutcome Game::robotAction(Action action, Robot robot) 
{
  if (action.secondaryActiontype != Action::nothing)
  {
    Move::moveOutcome result;
    Action primaryAction = action;
    Action secondaryAction = Action(action.secondaryActiontype, action.property21, action.property22);
    primaryAction.secondaryActiontype = Action::nothing;

    result = robotAction(primaryAction, robot);
    if (result.result != Move::success)
    {
      return result;
    }
    result = robotAction(secondaryAction, robot);
    if (result.result != Move::success)
    {
      undo(robot, primaryAction);
    }
    return result;
  }
  switch (action.type)
  {
    case (Action::damage):
    {
      robot.damage += action.property1;
      if (robot.damage > 9) return {Move::death, 0};
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
      if (robot.currentflag + 1 == action.property1)
      {
        robot.currentflag++;
      }
      robot.respawnx = robot.x;
      robot.respawny = robot.y;
      return {Move::success, 0};
    }
    case (Action::moveabs):
    {
      int newx = robot.x;
      int newy = robot.y;
      switch (action.property1)
      {
        case 0:
          newy++;
        case 1:
          newx++;
        case 2:
          newy--;
        case 3:
          newx--;
      }
      Move::moveOutcome tileresult = board[newx][newy].moveToHere(action.property1);
      if (tileresult.result != Move::success)
      {
        return tileresult;
      }
      for (int i = 0; i < nPlayers; i++)
      {
        if (robots[i].x == newx && robots[i].y == newy)
        {
          return {Move::fail, i};
        }
      }
      robot.x = newx;
      robot.y = newy;
      return {Move::success, 0};
    }
    case (Action::moverel):
    {
      for (int i = 0; i < action.property2; i++)
      {
        Move::moveOutcome result = robotAction(Action(Action::moveabs, (action.property1 + robot.facing)%4), robot);
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
      robot.facing += action.property1;
      robot.facing %= 4;
      return {Move::success, 0};
    }
    case (Action::setspawn):
    {
      robot.respawnx = robot.x;
      robot.respawny = robot.y;
      return {Move::success, 0};
    }
    case (Action::upgrade):
    {
      return {Move::success, 0};
    }
  }
}




