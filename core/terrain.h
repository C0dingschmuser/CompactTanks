#ifndef TERRAIN_H
#define TERRAIN_H
#include <QString>
#include <QRect>
#include <QDebug>
#include <QPixmap>

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
    QPixmap p;
    QPixmap grass;
public:
    Terrain();
    Terrain(int x, int y, int w, int h, int type=0,int itype=0);
    //void setPixmap(QPixmap p);
    void setType(int type);
    void setOwner(int owner);
    void setAmount(int amount);
    QRect getRect();
    QString toString();
    int getType();
    int getOwner();
    int getAmount();
    QPixmap getPixmap();
    QPixmap getGrassPixmap();
};

#endif // TERRAIN_H
