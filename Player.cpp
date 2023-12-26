#include "Player.h"

Player::Player(Robot *robot, Controls *controls)
{
    this->robot = robot;
    this->controls = controls;
}

Action Player::getCardAction(int registry)
{
    return cards[registry].getAction();
}

int Player::getCardPriority(int registry)
{
    return cards->number;
}

Action Player::getTileAction(int phase, int registry)
{
    return tile->getAction(phase, registry);
}

void Player::moveInDirection(int dir)
{
    controls->moveRobot(robot, utility::dx(dir), utility::dy(dir), 0);
    tile = tile->getNeighbor(dir);
}

int Player::getX()
{
    return robot->x;
}

int Player::getY()
{
    return robot->y;
}

bool Player::isDead()
{
    return robot->dead;
}

void Player::checkDead()
{
    if (robot->damage >= 10)
    {
        robot->dead = true;
    }
}

void Player::setCard(int index, Card card)
{
    cards[index] = card;
}
