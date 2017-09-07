#include "terrain.h"

Terrain::Terrain()
{

}

Terrain::Terrain(int x, int y, int w, int h, int type, int itype)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->type = type;
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

void Terrain::setAmount(int amount)
{
    this->cp = amount;
}

void Terrain::setOwner(int owner)
{
    this->owner = owner;
}
