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

QPixmap Terrain::getPixmap()
{
    return this->p;
}

QPixmap Terrain::getGrassPixmap()
{
    return this->grass;
}
