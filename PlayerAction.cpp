#include "PlayerAction.h"

PlayerAction::PlayerAction()
{
    // create a playeraction that will do nothing when executed
    this->nothingAction = true;
}

PlayerAction::PlayerAction(Action action, Player *player, Controls *controls)
{
    this->action = action;
    this->player = player;
    this->controls = controls;
}

bool PlayerAction::execute()
{
    if (player->isDead())
    {
        return true;
    }

    if (action.isComposite)
    {
        PlayerAction a1 = PlayerAction(*action.composite1, player, controls);
        PlayerAction a2 = PlayerAction(*action.composite2, player, controls);

        bool r1 = a1.execute();
        bool r2 = false;
        if (r1) r2 = a2.execute(); // do a2 if a1 was successful
        delete action.composite1;
        delete action.composite2;
        return r1 && r2; // if both are successful return true
    }
    else if (action.moveabs != -1)
    {
        Serial.print("Move absolute: ");
        Serial.println(action.moveabs);
        return moveAbs(action.moveabs, action.powerMove);
    }
    else if (action.moverel != -1)
    {
        Serial.print("Move relative: ");
        Serial.println(action.moverel);
        int direction = (player->robot->facing + action.moverel) % 4;
        return moveAbs(direction, action.powerMove);
    }
    else if (action.rotation != 0)
    {
        controls->moveRobot(player->robot, 0, 0, action.rotation);
    }
    else // no need to separate these
    {
        player->robot->damage += action.damage;
        player->checkDead();

        if (action.death) player->robot->dead = true;

        if (action.flag == player->robot->currentflag + 1) player->robot->currentflag++;

        if (action.setspawn) player->robot->setSpawn();

    }
    return true;
    
}

int PlayerAction::xDiff()
{
    if (action.isComposite)
    {
        return PlayerAction(*action.composite1, player, controls).xDiff() + PlayerAction(*action.composite2, player, controls).yDiff();
    }
    else if (action.moveabs != -1)
    {
        return utility::dx(action.moveabs);
    }
    else if (action.moverel != -1)
    {
        return utility::dx(action.moverel + player->robot->facing);
    }
    else
    {
        return 0;
    }
}

int PlayerAction::yDiff()
{
    if (action.isComposite)
    {
        return PlayerAction(*action.composite1, player, controls).yDiff() + PlayerAction(*action.composite2, player, controls).yDiff();
    }
    else if (action.moveabs != -1)
    {
        return utility::dy(action.moveabs);
    }
    else if (action.moverel != -1)
    {
        return utility::dy(action.moverel + player->robot->facing);
    }
    else
    {
        return 0;
    }
}

bool PlayerAction::moveAbs(int dir, bool powermove)
{
    move::moveOutcome outcome = player->tile->moveAway(dir);
    Serial.print("outcome is: ");
    switch (outcome)
    {
        case move::death:
            Serial.println("death");
            player->moveInDirection(dir);
            player->robot->dead = true;
            return true;
        
        case move::fail:
            Serial.println("fail");
            return false;
        
        case move::push:
            Serial.println("push");
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
            Serial.println("success");
            player->moveInDirection(dir);
            return true;
    }
    return true;
}

bool PlayerAction::pushOther(int dir)
{
    for (Player *other : players)
    {
        // if someone is standing at the new space
        if (utility::dx(action.moveabs) == other->getX() && utility::dy(action.moveabs) == other->getY())
        {
            PlayerAction otherAction = PlayerAction(action, other, controls);
            bool success = otherAction.execute();
            if (!success)
            {
                return false;
            }
        }
    }
    return true;
}
