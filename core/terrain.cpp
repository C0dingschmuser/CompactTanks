#include "terrain.h"

Terrain::Terrain()
{

}

Terrain::Terrain(int x, int y, int w, int h, int type)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->type = type;
    this->p = QPixmap(":/images/area/obj"+QString::number(type,'f',0)+".png");
    this->grass = QPixmap(":/images/area/obj9.png");
    owner = 0;
    cp = 0;
}


QRect Terrain::getRect()
{
    return QRect(x,y,w,h);
}

QString Terrain::toString()
{
    return QString::number(x,'f',0) + "#" + QString::number(y,'f',0) + "#" +
            QString::number(w,'f',0) + "#" + QString::number(h,'f',0) + "#";
}

int Terrain::getType()
{
    return this->type;
}

int Terrain::getOwner()
{
    return owner;
}

int Terrain::getAmount()
{
    return cp;
}

QPixmap Terrain::getPixmap()
{
    return this->p;
}

void Terrain::setAmount(int amount)
{
    this->cp = amount;
}

void Terrain::setOwner(int owner)
{
    this->owner = owner;
}

QPixmap Terrain::getGrassPixmap()
{
    return this->grass;
}
