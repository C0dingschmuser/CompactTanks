#include "terrain.h"

Terrain::Terrain()
{

}

Terrain::Terrain(int x, int y, int w, int h)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
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
