#include "Game.h"

// apparently a bug in arduino that this isn't always included by default I think
#define min(a,b) ((a)<(b)?(a):(b))

Game::Game() {
  Serial.println("Game constructor");
  board = new Board(&controls);
  Serial.println(board->robots[0]->facing);
  for (int i = 0; i < 8; i++)
  {
    players[i] = nullptr;
  }
}

Game::~Game()
{
  for (int i = 0; i < 8; i++)
  {
    delete players[i];
  }
  delete board;
}

void Game::init()
{
  Serial.println("Game init");
}


void Game::setNumberOfPlayers(int numberOfPlayers) {
  nPlayers = numberOfPlayers;
  for (int i=0; i<min(numberOfPlayers, 4); i++) {
    players[i] = new Player(board->getFreeRobot(players[i]), &controls, &defaultTile);
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
  board->setBoard(0, 0, false);
  input = Input(nPlayers, players);
  for (int i = 0; i < nPlayers; i++)
  {
    players[i]->robot->x = 1;
    players[i]->robot->y = 1;
    board->updatePlayerTile(players[i]);
  }
  controls.init();
  controls.home();

  Serial.println("Starting game");

  // for each round:
  while (true)
  {
    // phase 0

    // phase 1
    input.readCards();

    // phase 2
    input.readPowerDown(2);

    // phases 3-11
    for (int reg = 0; reg < 5; reg++)
    {
      for (int i = 0; i < nPlayers; i++)
      {
        Serial.print("Player ");
        Serial.print(i);
        Serial.print(" at ");
        Serial.print(players[i]->robot->x);
        Serial.print(",");
        Serial.print(players[i]->robot->y);
        Serial.print(",");
        Serial.println(players[i]->robot->facing);
      }
      Serial.println("Press enter to continue");
      Serial.readStringUntil('\n');
      // phase 3


      // phase 4
      playCards(reg);

      // phase 5-11
      for (int phase = 5; phase <= 11; phase++)
      {
        Serial.println("Phase " + String(phase));
        if (phase == 9) // lasers
        {
          board->zapLasers();
        }
        else {
          PlayerAction playerActions[nPlayers];
          for (int iPlayer = 0; iPlayer < nPlayers; iPlayer++)
          {
            Action* action = players[iPlayer]->getTileAction(phase, reg);
            playerActions[iPlayer] = PlayerAction(action, players[iPlayer], &controls, board);
            //if (phase == 5) players[iPlayer].tile->printProperties();
          }
          if (phase == 5 || phase == 6) // conveyors
          {
            for (int i = 0; i < nPlayers; i++)
            {
              int x = players[i]->getX() + playerActions[i].xDiff(), y = players[i]->getY() + playerActions[i].yDiff();
              for (int j = i+1; j < nPlayers; j++)
              {
                if (x == players[j]->getX() + playerActions[j].xDiff() && players[j]->getY() + playerActions[j].yDiff()) 
                {
                  playerActions[i] = PlayerAction();
                  playerActions[j] = PlayerAction();
                }
              }
            }
          }
          for (int i = 0; i < nPlayers; i++)
          {
            playerActions[i].execute();
          }
        }
      }
    }

    // phase 12
    for (int iPlayer = 0; iPlayer < nPlayers; iPlayer++) 
    {
      Action* action = players[iPlayer]->getTileAction(12, -1);
      PlayerAction playerAction = PlayerAction(action, players[iPlayer], &controls, board);
      playerAction.execute();
    }

    // phase 13
    // phase 14

    // phase 15
    input.readPowerDown(15);

    // phase 16
    board->reviveRobots();
  }
}


void Game::playCards(int reg)
{
  int iplayers[nPlayers];

  // find player order
  for (int i = 0; i < nPlayers; i++)
  {
    iplayers[i] = -1;
    for (int j = 0; j < nPlayers; j++)
    {
      if (i > 0 && players[iplayers[i-1]]->getCardPriority(reg) >= players[j]->getCardPriority(reg)) 
      {
        continue;
      }

      if (iplayers[i] == -1)
      {
        iplayers[i] = j;
      }
      else if (players[iplayers[i]]->getCardPriority(reg) < players[j]->getCardPriority(reg))
      {
        iplayers[i] = j;
      }
    }
  }

  // move robots
  for (int i = 0; i < nPlayers; i++)
  {
    //Serial.println("move robot start");
    //players[i].tile->printProperties();
    Player *player = players[iplayers[i]];
    if (player->isDead()) continue;
    Serial.println("Play card turn " + String(reg) + " robot " + String(iplayers[i]));
    Action* action = player->getCardAction(reg);
    PlayerAction playerAction = PlayerAction(action, player, &controls, board);
    playerAction.execute();
    //board.cleanDeadRobots();
    //players[i].tile->printProperties();
    //Serial.println("move robot end");
  }
}
