#ifndef WEAPON_H
#define WEAPON_H


class Weapon
{
private:
    int vel;
    int cooldown;
public:
    Weapon(int v,int c);
    int getVel();
    int getCoolDown();
};

#endif // WEAPON_H
