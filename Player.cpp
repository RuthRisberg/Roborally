#include "Player.h"

Player::~Player()
{
    for (int i = 0; i < 5; i++)
    {
        delete cards[i];
    }
}

Player::Player(Player&& other)
{
    tile = other.tile;
    robot = other.robot;
    powerDownNextRound = other.powerDownNextRound;
    controls = other.controls;
    for (int8_t i = 0; i < 5; i++)
    {
        cards[i] = other.cards[i];

        other.cards[i] = nullptr;
    }

    other.tile = nullptr;
    other.robot = nullptr;
    other.powerDownNextRound = false;
    other.controls = nullptr;
}

Player& Player::operator=(Player&& other)
{
    if (this != &other)
    {
        delete tile;
        delete robot;
        delete controls;
        for (int i = 0; i < 5; i++)
        {
            delete cards[i];
        }

        tile = other.tile;
        robot = other.robot;
        powerDownNextRound = other.powerDownNextRound;
        controls = other.controls;
        for (int i = 0; i < 5; i++)
        {
            cards[i] = other.cards[i];
        }

        other.tile = nullptr;
        other.robot = nullptr;
        other.powerDownNextRound = false;
        other.controls = nullptr;
    }
    return *this;
}

Player::Player()
{
    tile = nullptr;
    robot = nullptr;
    powerDownNextRound = false;
    controls = nullptr;
    for (int i = 0; i < 5; i++)
    {
        cards[i] = nullptr;
    }
}

Player::Player(Robot *robot, Controls *controls, Tile *tile)
{
    this->robot = robot;
    this->controls = controls;
    this->tile = tile;

    powerDownNextRound = false;
    for (int i = 0; i < 5; i++)
    {
        cards[i] = nullptr;
    }
}

Action* Player::getCardAction(int registry)
{
    return cards[registry]->getAction();
}

int Player::getCardPriority(int registry)
{
    return cards[registry]->number;
}

Action* Player::getTileAction(int phase, int registry)
{
    return tile->getAction(phase, registry);
}

void Player::moveInDirection(int dir)
{
    controls->moveRobot(robot, utility::dx(dir), utility::dy(dir), 0);
    tile->removeRobot();
    tile = tile->getNeighbor(dir);
    tile->setRobot(robot);
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

void Player::setCard(int index, Card *card)
{
    delete cards[index];
    cards[index] = card;
}
