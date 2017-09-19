#ifndef TERRAIN_H
#define TERRAIN_H
#include <QString>
#include <QRect>
#include <QDebug>
#include <QPixmap>
#include "Box2D/Box2D.h"

class Terrain
{
private:
    int x;
    int y;
    int w;
    int h;
    int type;
    int owner;
    int cp;
    b2Body *body;
public:
    Terrain();
    Terrain(int x, int y, int w, int h, b2World *world,int type=0,int itype=0);
    //void setPixmap(QPixmap p);
    void setType(int type);
    void setOwner(int owner);
    void setAmount(int amount);
    QRect getRect();
    QString toString();
    int getType();
    int getOwner();
    int getAmount();
};

#endif // TERRAIN_H
