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
    int team;
    int coins;
    bool moved;
    bool visible;
    QVector <QPixmap> imgs;
    QPixmap currentImg;
    QPixmap grid;
    QPoint targetPos;
public:
    Tank();
    Tank(QRect rect, QString name, int team=-1);
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
    int getTeam();
    int getCoins();
    void setVisible(bool visible);
    void teleport(int x,int y);
    void setAll(int x, int y, int dir, int health);
    void setMoved(bool m);
    void setHealth(int health);
    void setTeam(int team);
    void setCoins(int coins);
    void drawTank(QPainter &p, Tank *own, bool barrel=false);
    void move();
    bool getVisible();
    QString getName();
    QString toString();
    QPixmap getIMG();
    QLineF getBarrel(int xt, int yt);
};

#endif // TANK_H
