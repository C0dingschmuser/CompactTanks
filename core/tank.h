#ifndef TANK_H
#define TANK_H
#include <QRect>
#include <QtMath>
#include <QDebug>
#include <QString>
#include <QVector>
#include <QPointF>
#include <QLineF>
#include <QFont>
#include <QPixmap>
#include <QPainter>

class Tank
{
private:
    QRect rect;
    QString name;
    int color;
    int speed;
    int kills;
    int deaths;
    int dir;
    int viewRange;
    int angle;
    int health;
    bool moved;
    bool visible;
    QVector <QPixmap> imgs;
    QPixmap currentImg;
    QPoint targetPos;
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
    void setColor(int color);
    void setAngle(int angle);
    int getSpeed();
    int getKills();
    int getDeaths();
    int getDir();
    int getHealth();
    int getAngle();
    void setVisible(bool visible);
    void teleport(int x,int y);
    void setAll(int x, int y, int dir, int health);
    void setMoved(bool m);
    void setHealth(int health);
    void drawTank(QPainter &p,bool barrel=false);
    void move();
    bool getVisible();
    QString getName();
    QString toString();
    QPixmap getIMG();
    QLineF getBarrel();
};

#endif // TANK_H
