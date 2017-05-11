#include "player.h"

Player::Player()
{

}

Player::Player(QString n)
{
    this->name = n;
}

Player::~Player()
{

}

void Player::setName(QString n)
{
    this->name = n;
}

QString Player::getName()
{
    return this->name;
}

