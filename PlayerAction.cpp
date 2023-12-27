#include "PlayerAction.h"

PlayerAction::PlayerAction()
{
    // create a playeraction that will do nothing when executed
    this->nothingAction = true;
}

PlayerAction::PlayerAction(Action *action, Player *player, Controls *controls, Board *board)
{
    this->action = action;
    this->player = player;
    this->controls = controls;
    this->board = board;
}

bool PlayerAction::execute()
{
    Serial.println("Executing action...");
    Serial.println(nothingAction);
    bool retval = true;
    if (player->isDead())
    {
        Serial.println("dead");
        retval = true;
    }
    else if (action->isComposite)
    {
        Serial.println("Composite action");
        PlayerAction a1 = PlayerAction(action->composite1, player, controls, board);
        PlayerAction a2 = PlayerAction(action->composite2, player, controls, board);

        bool r1 = a1.execute();
        bool r2 = false;
        if (r1) r2 = a2.execute(); // do a2 if a1 was successful
        else delete action->composite2;
        retval = r1 && r2; // if both are successful return true
    }
    else if (action->moveabs != -1)
    {
        Serial.print("Move absolute: ");
        Serial.println(action->moveabs);
        retval = moveAbs(action->moveabs, action->powerMove);
    }
    else if (action->moverel != -1)
    {
        Serial.print("Move relative: ");
        Serial.println(action->moverel);
        int direction = (player->robot->facing + action->moverel) % 4;
        retval = moveAbs(direction, action->powerMove);
    }
    else if (action->rotation != 0)
    {
        Serial.println("rotate");
        controls->moveRobot(player->robot, 0, 0, action->rotation);
    }
    else // no need to separate these
    {
        Serial.println("other");
        player->robot->damage += action->damage;
        player->checkDead();

        if (action->death) 
        {
            player->robot->dead = true;
        }

        if (player->isDead())
        {
            board->cleanDeadRobot(player);
        }

        if (action->flag == player->robot->currentflag + 1) 
        {
            player->robot->currentflag++;
        }

        if (action->setspawn) 
        {
            player->robot->setSpawn();
        }

    }
    delete action;
    return true;
}

int PlayerAction::xDiff()
{
    if (action->isComposite)
    {
        return PlayerAction(action->composite1, player, controls, board).xDiff() + PlayerAction(action->composite2, player, controls, board).yDiff();
    }
    else if (action->moveabs != -1)
    {
        return utility::dx(action->moveabs);
    }
    else if (action->moverel != -1)
    {
        return utility::dx(action->moverel + player->robot->facing);
    }
    else
    {
        return 0;
    }
}

int PlayerAction::yDiff()
{
    if (action->isComposite)
    {
        return PlayerAction(action->composite1, player, controls, board).yDiff() + PlayerAction(action->composite2, player, controls, board).yDiff();
    }
    else if (action->moveabs != -1)
    {
        return utility::dy(action->moveabs);
    }
    else if (action->moverel != -1)
    {
        return utility::dy(action->moverel + player->robot->facing);
    }
    else
    {
        return 0;
    }
}

bool PlayerAction::moveAbs(int dir, bool powermove)
{
    bool print = true;
    move::moveOutcome outcome = player->tile->moveAway(dir);
    if(print) Serial.print("outcome is: ");
    switch (outcome)
    {
        case move::edge:
            if(print) Serial.println("edge");
            player->robot->dead = true;
            board->cleanDeadRobot(player);
            return true;
            
        case move::death:
            if(print) Serial.println("death");
            player->moveInDirection(dir);
            player->robot->dead = true;
            board->cleanDeadRobot(player);
            return true;
        
        case move::fail:
            if(print) Serial.println("fail");
            return false;
        
        case move::push:
            if(print) Serial.println("push");
            if (powermove)
            {
                bool success = pushOther(dir);
                if (!success)
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        case move::success:
            if(print) Serial.println("success");
            player->moveInDirection(dir);
            return true;
    }
    return true;
}

// TODO: this doesn't seem to account for relative moves where the other robot is facing another direction
bool PlayerAction::pushOther(int dir)
{
    for (Player *other : players)
    {
        // if someone is standing at the new space
        if (utility::dx(action->moveabs) == other->getX() && utility::dy(action->moveabs) == other->getY())
        {
            PlayerAction otherAction = PlayerAction(action, other, controls, board);
            bool success = otherAction.execute();
            if (!success)
            {
                return false;
            }
        }
    }
    return true;
}
