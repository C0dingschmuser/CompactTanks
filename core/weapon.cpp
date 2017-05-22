#include "weapon.h"

Weapon::Weapon(int v, int c)
{
    this->vel = v;
    this->cooldown = c;
}

int Weapon::getCoolDown()
{
    return this->cooldown;
}

int Weapon::getVel()
{
    return this->vel;
}
