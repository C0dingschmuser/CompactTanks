#ifndef BULLET_H
#define BULLET_H
#include <QtMath>
#include <QRect>
#include <QColor>
#include <QDebug>
class Bullet
{
public:
    Bullet();
    ~Bullet();
    Bullet(int x, int y, double x2, double y2, int vel, QString shooter, QColor color = Qt::black,int onImpact = 0);
    void setColl(bool coll);
    void update();
    void sync(int x, int y, int elapsed=-1);
    void setDmg(int dmg);
    bool getColl();
    QString getShooter();
    QRect get();
    QColor getColor();
    int getImpactID();
    int getElapsed();
    int getVel();
    int getDmg();
private:
    QColor color;
    double x;
    double y;
    double x2;
    double y2;
    int vel;
    int onImpact;
    QString shooter;
    int elapsed;
    int dmg;
    bool coll;
    double getDistance(QPoint p1,QPoint p2);
};

#endif // BULLET_H
