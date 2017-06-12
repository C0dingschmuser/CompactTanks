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

class Worker : public QObject
{
    Q_OBJECT
private slots:
    void on_newPlayer(Tank *t);
    void on_delPlayer(int pos);
    void on_newlvlObj(int x,int y,int w,int h,int type);
    void on_newBullet(Bullet *b);
    void on_delBullet(int pos);
    void on_syncBullet(int pos,int x,int y, int elapsed);
    void on_delObjs();
    void on_tbullet();
    //void on_death();
    //void on_kick(); dl
    //void on_fullscreen(); //dl braucht connect mit move
    void on_visible(int v);
    void on_capobj(int num,int owner,int cp);
private:
    QRect viewRect;
    QTimer *t_bullet;
    Tank *ownTank;
    QVector <Tank*> tanks;
    QVector <Terrain*> lvlObjs;
    QVector <Bullet*> bullets;
    Movement *move;
    Network *network;
    Shoot *shoot;
    QPoint *aim;
    int width;
    int height;
    void loadMap();
    int getType(int type);
public:
    explicit Worker(Tank *ownTank,QPoint *aim, int width, int height, QObject *parent = 0);
    ~Worker();
    void keyP(QKeyEvent *e);
    void keyR(QKeyEvent *e);
    void mPrs(QMouseEvent *e);
    void mRls(QMouseEvent *e);
    void notActive();
    void setViewRect(QRect viewRect);
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
};

#endif // WORKER_H
