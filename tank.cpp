#include "tank.h"

Tank::Tank()
{

}

Tank::Tank(QRect rect, QString name)
{
    this->rect = rect;
    this->name = name;
    speed = 3;
    dir = 1;
    kills = 0;
    deaths = 0;
    moved = true;
    viewRange = 120;
}

Tank::~Tank()
{

}

bool Tank::getMoved()
{
    return this->moved;
}

QRect Tank::getRect()
{
    return this->rect;
}

void Tank::w()
{
    this->dir = 1;
    this->rect.moveTo(rect.x(),rect.y()-speed);
}

void Tank::a()
{
    this->dir = 2;
    this->rect.moveTo(rect.x()-speed,rect.y());
}

void Tank::s()
{
    this->dir = 3;
    this->rect.moveTo(rect.x(),rect.y()+speed);
}

void Tank::d()
{
    this->dir = 4;
    this->rect.moveTo(rect.x()+speed,rect.y());
}

void Tank::kill()
{
    kills++;
}

void Tank::death()
{
    deaths++;
}

void Tank::setDK(int kills, int deaths)
{
    this->kills = kills;
    this->deaths = deaths;
}

QString Tank::getName()
{
    return this->name;
}

QString Tank::toString()
{
    return this->name + "#" + QString::number(rect.x(),'f',0)+"#"+QString::number(rect.y(),'f',0)+"#"+
            QString::number(dir,'f',0)+"#";
}

void Tank::setAll(int x, int y, int dir)
{
    this->dir = dir;
    this->rect.moveTo(x,y);
}

void Tank::setMoved(bool m)
{
    this->moved = m;
}

void Tank::setSpeed(int speed)
{
    this->speed = speed;
}

int Tank::getSpeed()
{
    return this->speed;
}

int Tank::getKills()
{
    return this->kills;
}

int Tank::getDeaths()
{
    return this->deaths;
}

int Tank::getDir()
{
    return this->dir;
}

int Tank::getViewRange()
{
    return this->viewRange;
}

void Tank::setViewRange(int vr)
{
    this->viewRange = vr;
}
