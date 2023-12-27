#include "Input.h"

Input::Input(int nPlayers, Player **players)
{
    this->nPlayers = nPlayers;
    this->players = players;
}

void Input::readCards()
{
    //Serial.println("Format for cards is '<back/forwardX/left/right/uturn> <priority (3 digits)>', ex: 'forward2 270' or 'uturn 070'");
    for (int i = 0; i < nPlayers; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            Serial.println("\nPlayer " + String(i) + " card " + String(j) + ":");
            while (!Serial.available());
            String inp = Serial.readStringUntil('\n');
            Action* action;
            Serial.print("Card chosen: ");
            switch (inp[0])
            {
                case 'b':
                    {action = new RelativeMoveAction(2);
                    Serial.print("move backwards");
                    break;}
                
                case 'f':
                   {int number;
                    if (inp.substring(1,1) == "o")
                    {
                        number = stringToInt(inp.substring(7, 8));
                    }
                    else
                    {
                        number = stringToInt(inp.substring(1, 2));
                    }
                    action = new MoveForwardAction(number);
                    Serial.print("move " + String(number) + " forwards");
                    break;}
                
                case 'l':
                    {action = new RotateAction(-1);
                    Serial.print("rotate left");
                    break;}
                
                case 'r':
                    {action = new RotateAction(1);
                    Serial.print("rotate right");
                    break;}

                case 'u':
                    {action = new RotateAction(2);
                    Serial.print("U-turn");
                    break;}
                
                default:
                    {Serial.println("Invalid card. Try again.");
                    Serial.println("Format for cards is '<back/forwardX/left/right/uturn> <priority (3 digits)>', ex: 'forward2 270' or 'uturn 070'");
                    j--;
                    continue;}
            }
            int priority = stringToInt(inp.substring(inp.length() - 4));
            Serial.println(" with priority " + String(priority));


            players[i]->setCard(j, new Card(action, priority));
        }
    }
}

void Input::readPowerDown(int phase)
{
    switch (phase)
    {
        case 2:
            for (int i = 0; i < nPlayers; i++)
            {
                if (!players[i]->robot->poweredDown)
                {
                    Serial.println("Does player " + String(i) + "want to power down next round? (y/n)");
                    while (Serial.available() == 0);
                    if (Serial.readStringUntil('\n').charAt(0) == 'y')
                    {
                        players[i]->powerDownNextRound = true;
                    }
                }
            }
            break;

        case 15:
            for (int i = 0; i < nPlayers; i++)
            {
                if (players[i]->robot->poweredDown)
                {
                    Serial.println("Does player " + String(i) + "want to keep power down next round? (y/n)");
                    while (Serial.available() == 0);
                    if (Serial.readStringUntil('\n').charAt(0) == 'y')
                    {
                        players[i]->powerDownNextRound = true;
                    }
                    else
                    {
                        players[i]->robot->poweredDown = false;
                    }
                }

                if (players[i]->powerDownNextRound)
                {
                    players[i]->powerDownNextRound = false;
                    players[i]->robot->poweredDown = true;
                }
            }
            break;

        default:
            Serial.println("readPowerDown called with invalid argument " + String(phase) + ".");
    }
}
