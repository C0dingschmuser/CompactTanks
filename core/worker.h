#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QFile>
#include <QStringList>
#include <QVector>
#include "core/network.h"
#include "core/movement.h"
#include "core/shoot.h"
#include "core/terrain.h"
#include "core/sound.h"

class Worker : public QObject
{
    Q_OBJECT
private slots:
    void on_newPlayer(Tank *t);
    void on_delPlayer(int pos);
    void on_newlvlObj(int x,int y,int w,int h,int type);
    void on_newBullet(Bullet *b);
    void on_delBullet(int pos);
    void on_syncBullet(int pos,int dmg);
    void on_delObjs();
    void on_tbullet();
    void on_tmain();
    void on_tconn();
    //void on_death();
    //void on_kick(); dl
    //void on_fullscreen(); //dl braucht connect mit move
    void on_visible(int v);
    void on_capobj(int num,int owner,int cp);
    void on_setT(int timer);
    void on_pos(Tank *p,int x,int y,int dir,int health, int angle,int spotted,int stimer);
    void on_conn(bool success);
    void on_tid();
private:
    QRect viewRect;
    QTimer *t_bullet;
    QTimer *t_main;
    QTimer *t_conn;
    QTimer *t_id;
    Tank *ownTank;
    QVector <Tank*> tanks;
    QVector <Terrain*> lvlObjs;
    QVector <Bullet*> bullets;
    Movement *move;
    Network *network;
    Shoot *shoot;
    QPoint *aim;
    QString username;
    int width;
    int height;
    int timer;
    void loadMap();
    int getType(int type);
    int getDifference(int v1,int v2);
public:
    explicit Worker(Tank *ownTank,QPoint *aim, int width, int height, QObject *parent = 0);
    ~Worker();
    void keyP(QKeyEvent *e);
    void keyR(QKeyEvent *e);
    void mPrs(QMouseEvent *e);
    void mRls(QMouseEvent *e);
    void notActive();
    void setViewRect(QRect viewRect);
    void connectToServer(QString username,QString password);
signals:
    void newPlayer(Tank *t);
    void delPlayer(int pos);
    void newlvlObj(Terrain *t);
    void newMap(QVector<Terrain*> lvlObjs);
    void delObjs();
    void newBullet(Bullet *b);
    void delBullet(int pos);
    void disconnected();
    void message(QString message, int length);
    void killMessage(QString message);
    void kick();
    void fullscreen();
    void visible(bool visible);
    void tab();
    void connFail();
    void connSuccess();
    void wrongData();
    void shot();
    void hit(Tank *t, int dmg);
};

#endif // WORKER_H
