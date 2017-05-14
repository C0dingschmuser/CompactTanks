#ifndef BULLET_H
#define BULLET_H
#include <QtMath>
#include <QRect>
#include <QColor>
class Bullet
{
public:
    Bullet();
    ~Bullet();
    Bullet(int x, int y, double x2, double y2, int vel, QString shooter, QColor color = Qt::black,int onImpact = 0);
    void setColl(bool coll);
    void update();
    void sync(int x, int y, int elapsed);
    bool getColl();
    QString getShooter();
    QRect get();
    QColor getColor();
    int getImpactID();
    int getElapsed();
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
    bool coll;
};

#endif // BULLET_H
