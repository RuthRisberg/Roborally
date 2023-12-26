#include "Game.h"

// apparently a bug in arduino that this isn't always included by default I think
#define min(a,b) ((a)<(b)?(a):(b))

Game::Game() {}

void Game::init()
{
}


void Game::setNumberOfPlayers(int numberOfPlayers) {
  nPlayers = numberOfPlayers;
  for (int i=0; i<min(numberOfPlayers, 4); i++) {
    players[i] = Player(board.getFreeRobot(), &controls);
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
  board = Board();
  board.setBoard(0, 0);
  input = Input(nPlayers, players);
  for (int i = 0; i < nPlayers; i++)
  {
    players[i].robot->x = 1;
    players[i].robot->y = 1;
    board.updatePlayerTile(&players[i]);
  }
  controls.init();

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
        Serial.print(players[i].robot->x);
        Serial.print(",");
        Serial.print(players[i].robot->y);
        Serial.print(",");
        Serial.println(players[i].robot->facing);
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
          board.zapLasers();
        }
        else {
          PlayerAction playerActions[nPlayers];
          for (int iPlayer = 0; iPlayer < nPlayers; iPlayer++)
          {
            Action action = players[iPlayer].getTileAction(phase, reg);
            playerActions[iPlayer] = PlayerAction(action, &players[iPlayer], &controls);
            //if (phase == 5) players[iPlayer].tile->printProperties();
          }
          if (phase == 5 || phase == 6) // conveyors
          {
            for (int i = 0; i < nPlayers; i++)
            {
              int x = players[i].getX() + playerActions[i].xDiff(), y = players[i].getY() + playerActions[i].yDiff();
              for (int j = i+1; j < nPlayers; j++)
              {
                if (x == players[j].getX() + playerActions[j].xDiff() && players[j].getY() + playerActions[j].yDiff()) 
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
        for (int i = 0; i < nPlayers; i++)
        {
          players[i].checkDead();
        }
        board.cleanDeadRobots();
      }
    }

    // phase 12
    for (int iPlayer = 0; iPlayer < nPlayers; iPlayer++) 
    {
      Action action = players[iPlayer].getTileAction(12, -1);
      PlayerAction playerAction = PlayerAction(action, &players[iPlayer], &controls);
      playerAction.execute();
    }

    // phase 13
    // phase 14

    // phase 15
    input.readPowerDown(15);

    // phase 16
    board.reviveRobots();
  }
}



/* void Game::readcards()
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
} */


void Game::playCards(int reg)
{
  int iplayers[nPlayers];

  // find player order
  for (int i = 0; i < nPlayers; i++)
  {
    iplayers[i] = -1;
    for (int j = 0; j < nPlayers; j++)
    {
      if (i > 0 && players[iplayers[i-1]].getCardPriority(reg) >= players[j].getCardPriority(reg)) 
      {
        continue;
      }

      if (iplayers[i] == -1)
      {
        iplayers[i] = j;
      }
      else if (players[iplayers[i]].getCardPriority(reg) < players[j].getCardPriority(reg))
      {
        iplayers[i] = j;
      }
    }
  }

  // move robots
  for (int i = 0; i < nPlayers; i++)
  {
    Player *player = &players[iplayers[i]];
    if (player->isDead()) continue;
    Serial.println("Play card turn " + String(reg) + " robot " + String(iplayers[i]));
    Action action = player->getCardAction(reg);
    PlayerAction playerAction = PlayerAction(action, player, &controls);
    playerAction.execute();
    board.cleanDeadRobots();
  }
}
