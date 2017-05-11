#ifndef TERRAIN_H
#define TERRAIN_H
#include <QString>
#include <QRect>

class Terrain
{
private:
    int x;
    int y;
    int w;
    int h;
public:
    Terrain();
    Terrain(int x,int y,int w,int h);
    QRect getRect();
    QString toString();
};

#endif // TERRAIN_H
