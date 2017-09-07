#include "powerup.h"

Powerup::Powerup()
{

}

Powerup::Powerup(QRect rect, int type, int team)
{
    this->rect = rect;
    this->type = type;
    this->team = team;
    pixmap = QPixmap(":/images/powerup/"+QString::number(type)+".png");
}

QRect Powerup::getRect()
{
    return rect;
}

int Powerup::getType()
{
    return type;
}

int Powerup::getTeam()
{
    return team;
}

QPixmap Powerup::getPixmap()
{
    return pixmap;
}
