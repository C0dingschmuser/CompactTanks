#ifndef TANK_H
#define TANK_H
#include <QRect>
#include <QtMath>
#include <QDebug>
#include <QString>
#include <QVector>
#include <QPointF>

class Tank
{
private:
    QRect rect;
    QString name;
    int speed;
    int color;
    int kills;
    int deaths;
    int dir;
    bool moved;
public:
    Tank();
    Tank(QRect rect, QString name);
    ~Tank();
    bool getMoved();
    QRect getRect();
    void w();
    void a();
    void s();
    void d();
    void kill();
    void death();
    void setDK(int kills,int deaths);
    void setSpeed(int speed);
    int getSpeed();
    int getKills();
    int getDeaths();
    int getDir();
    void setAll(int x, int y, int dir);
    void setMoved(bool m);
    QString getName();
    QString toString();
};

#endif // TANK_H
