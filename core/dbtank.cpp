#include "dbtank.h"

dbTank::dbTank(QString name, int dmg, int reload, int speed, int health, int width, int height, int barrelLength, double softTerrRes, double hardTerrRes, double treeTerrRes, int treeColl, int vel, int camo, int viewrange)
{
    this->camo = camo;
    this->viewrange = viewrange;
    this->name = name;
    this->dmg = dmg;
    this->reload = reload;
    this->speed = speed;
    this->health = health;
    this->width = width;
    this->height = height;
    this->barrelLength = barrelLength;
    this->softTerrRes = softTerrRes;
    this->hardTerrRes = hardTerrRes;
    this->treeTerrRes = treeTerrRes;
    this->treeColl = treeColl;
    this->vel = vel;
}

QString dbTank::toString()
{
    QString r;
    r = QString::number(dmg,'f',0) + "#" + QString::number(reload,'f',0) + "#" + QString::number(speed,'f',0) + "#" + QString::number(health,'f',0) + "#" +
        QString::number(width,'f',0) + "#" + QString::number(height,'f',0) + "#" + QString::number(barrelLength,'f',0) + "#" + QString::number(softTerrRes,'f',2) + "#" +
        QString::number(hardTerrRes,'f',2) + "#" + QString::number(treeTerrRes,'f',2) + "#" + QString::number(treeColl,'f',0) + "#" + QString::number(vel,'f',0) +"#" +
            QString::number(camo,'f',0) + "#" + QString::number(viewrange,'f',0) +"#~";
    return r;
}

QString dbTank::getName()
{
    return name;
}

int dbTank::getDmg()
{
    return dmg;
}

int dbTank::getReload()
{
    return reload;
}

int dbTank::getSpeed()
{
    return speed;
}

int dbTank::getHealth()
{
    return health;
}

int dbTank::getWidth()
{
    return width;
}

int dbTank::getHeight()
{
    return height;
}

int dbTank::getBarrelLength()
{
    return barrelLength;
}

int dbTank::getCamo()
{
    return this->camo;
}

int dbTank::getViewrange()
{
    return viewrange;
}

double dbTank::getSoftTerrRes()
{
    return softTerrRes;
}

double dbTank::getHardTerrRes()
{
    return hardTerrRes;
}

double dbTank::getTreeTerrRes()
{
    return treeTerrRes;
}

int dbTank::getTreeColl()
{
    return treeColl;
}

int dbTank::getVel()
{
    return vel;
}
