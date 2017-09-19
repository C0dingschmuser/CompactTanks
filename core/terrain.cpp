#include "terrain.h"

Terrain::Terrain()
{
    body = nullptr;
}

Terrain::Terrain(int x, int y, int w, int h, b2World *world, int type, int itype)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->type = type;
    owner = 0;
    cp = 0;
    if(!type) {
        b2BodyDef def;
        def.type = b2_staticBody;
        def.position.Set(x+(w/2),y+(h/2));
        def.angle = 0;
        body = world->CreateBody(&def);
        b2PolygonShape boxShape;
        boxShape.SetAsBox(72/2,72/2);
        b2FixtureDef fixDef;
        fixDef.shape = &boxShape;
        fixDef.density = 2;
        body->CreateFixture(&fixDef);
    } else {
        body = nullptr;
    }
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
