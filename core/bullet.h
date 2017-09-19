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
    Bullet(int x, int y, double x2, double y2, int vel, QString shooter, bool heal = false, QColor color = Qt::black);
    void setColl(bool coll);
    void update();
    void sync(int x, int y, int elapsed=-1);
    void setDmg(int dmg);
    void setEnabled(bool enabled);
    void setTarget(int target=1);
    bool getColl();
    bool getEnabled();
    QString getShooter();
    QRect get();
    QColor getColor();
    int getImpactID();
    int getElapsed();
    int getVel();
    int getDmg();
    int getTarget();
private:
    QColor color;
    double x;
    double y;
    double x2;
    double y2;
    int vel;
    bool heal;
    QString shooter;
    int elapsed;
    int dmg;
    int target;
    bool coll;
    bool enabled;
    double getDistance(QPoint p1,QPoint p2);
};

#endif // BULLET_H
