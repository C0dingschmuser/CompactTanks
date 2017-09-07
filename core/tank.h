#ifndef TANK_H
#define TANK_H
#include <QRect>
#include <QtMath>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QVector>
#include <QPointF>
#include <QLineF>
#include <QFont>
#include <QPixmap>
#include <QRawFont>
#include <QPainter>

class Tank : public QObject
{
    Q_OBJECT
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
    bool heal;
    int vehicleID;
    int camo;
    double endSpeed;
    QPoint deathPoint;
    bool moved;
    bool visible;
    bool hidden;
    bool spawned;
    QPixmap img;
    QPixmap currentImg;
    QPixmap grid;
    QPixmap turret;
    QPoint targetPos;
    QPoint barrelStart;
public:
    explicit Tank(QRect rect, QString name, int team=-1, QObject *parent=0);
    ~Tank();
    bool getMoved();
    QRect getRect();
    void w(bool a=false);
    void a(bool a=false);
    void s(bool a=false);
    void d(bool a=false);
    void kill();
    void death();
    void setDK(int kills,int deaths);
    void setSpeed(int speed);
    void setColor(int color);
    void setAngle(int angle);
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
    int getDamage();
    int getSpeed();
    int getTimer();
    int getKills();
    int getDeaths();
    int getDir();
    int getHealth(int type=0);
    int getAngle();
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
    QPoint getDeathPoint();
    QPoint getShootPoint();
    void setVisible(bool visible);
    void teleport(int x,int y);
    void setAll(int x, int y, int dir, int health, int diff);
    void setAll(int x,int y);
    void setMoved(bool m);
    void setHealth(int health, int maxHealth=0);
    void setTeam(int team);
    void setCoins(int coins);
    void setVehicleID(int id);
    void drawTank(QPainter &p, Tank *own, bool barrel=false);
    void move();
    bool getVisible();
    bool isSpawned();
    bool isHidden();
    bool getHeal();
    QString getName();
    QString toString();
    QPixmap getIMG();
    QLineF getBarrel(int xt, int yt);
};

#endif // TANK_H
