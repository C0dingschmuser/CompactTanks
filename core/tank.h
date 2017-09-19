#ifndef TANK_H
#define TANK_H
#include <QRectF>
#include <QtMath>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QVector>
#include <QPointF>
#include <QLineF>
#include <QMatrix>
#include <QFont>
#include <QtMath>
#include <QPixmap>
#include <QRawFont>
#include <QPolygonF>
#include <QPainter>
#include "Box2D/Box2D.h"

class Tank : public QObject
{
    Q_OBJECT
private:
    QRectF rect;
    QString name;
    double vx,vy;
    int color;
    double speed;
    int kills;
    int deaths;
    int dir;
    int viewRange;
    double angle;
    int health;
    int team;
    int coins;
    int spotted;
    int currentID;
    int type;
    int timer;
    int damage;
    int bvel;
    int maxHealth;
    int reload;
    int width;
    int height;
    int barrelLength;
    int shootmode;
    int turnAngle;
    bool heal;
    int vehicleID;
    int camo;
    double endSpeed;
    QPoint deathPoint;
    bool moved;
    bool visible;
    bool hidden;
    bool spawned;
    bool stationary;
    bool locked;
    b2Body *body;
    QPixmap img;
    QPixmap currentImg;
    QPixmap grid;
    QPixmap turret;
    QPoint targetPos;
    QPoint barrelStart;
    QPoint getCenter();
public:
    explicit Tank(QRect rect, QString name, b2World *world, int team=-1, bool locked=true,QObject *parent=0);
    ~Tank();
    bool getMoved();
    QRectF getRect();
    b2Body *getBody();
    void w(bool a=false);
    void a(bool a=false);
    void s(bool a=false);
    void d(bool a=false);
    void kill();
    void death();
    void setDK(int kills,int deaths);
    void setSpeed(int speed);
    void setColor(int color);
    void setAngle(double angle);
    void setSpotted(int spotted);
    void setName(QString name);
    void setID(int id);
    void setSpawned(bool spawned);
    void setDeathPoint(QPoint dPoint);
    void setType(int type);
    void setData(int type,int speed, int health, int bvel, int reload,int width,int height,int barrelLength,bool heal, int camo, int viewrange, int vehicleID);
    void loadImages();
    void setDamage(int dmg);
    void setHidden(bool hidden);
    void setUsername(QString text);
    void setShootmode(int shootmode);
    void setTurnAngle(int angle);
    void setDir(int dir);
    int getShootmode();
    int getDamage();
    double getSpeed();
    int getTimer();
    int getKills();
    int getDeaths();
    int getDir();
    int getHealth(int type=0);
    double getAngle();
    int getTeam();
    int getCoins();
    int getSpotted();
    int getDifference(int v1,int v2);
    int getID();
    int getType();
    int getReload();
    int getBvel();
    int getWidth();
    int getHeight();
    int getEndSpeed();
    int getViewrange();
    int getVehicleID();
    int getTurnAngle();
    double getVx();
    double getVy();
    QPoint getDeathPoint();
    QPoint getShootPoint();
    void setVisible(bool visible);
    void teleport(int x,int y);
    void setAll(double x, double y, int turnAngle, int health, int diff, double vx, double vy);
    void setAll(double x,double y);
    void setMoved(bool m);
    void setHealth(int health, int maxHealth=0);
    void setTeam(int team);
    void setCoins(int coins);
    void setVehicleID(int id);
    void drawTank(QPainter &p, Tank *own, bool barrel=false);
    void move(int type=0);
    bool getVisible();
    bool isSpawned();
    bool isHidden();
    bool isStationary();
    bool getHeal();
    QPolygonF getPolygon(int type);
    QString getName();
    QString toString();
    QPixmap getIMG();
    QLineF getBarrel(int xt, int yt);
};

#endif // TANK_H
