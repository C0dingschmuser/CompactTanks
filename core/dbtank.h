#ifndef DBTANK_H
#define DBTANK_H
#include <QString>
#include <QDebug>

class dbTank
{
private:
    QString name;
    int dmg;
    int reload;
    int speed;
    int health;
    int width;
    int height;
    int barrelLength;
    int camo;
    int viewrange;
    double softTerrRes;
    double hardTerrRes;
    double treeTerrRes;
    int treeColl;
    int vel;
public:
    dbTank(QString name,int dmg, int reload, int speed, int health, int width, int height, int barrelLength, double softTerrRes, double hardTerrRes, double treeTerrRes, int treeColl, int vel, int camo, int viewrange);
    QString getName();
    int getDmg();
    int getReload();
    int getSpeed();
    int getHealth();
    int getWidth();
    int getHeight();
    int getBarrelLength();
    int getCamo();
    int getViewrange();
    double getSoftTerrRes();
    double getHardTerrRes();
    double getTreeTerrRes();
    int getTreeColl();
    int getVel();
    QString toString();
};

#endif // DBTANK_H
