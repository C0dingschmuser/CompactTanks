#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QFile>
#include <QStringList>
#include <QVector>
#include <QFont>
#include <QThread>
#include "core/network.h"
#include "core/movement.h"
#include "core/shoot.h"
#include "core/terrain.h"
#include "core/sound.h"
#include "core/dbtank.h"
#include "ui/frmtanks.h"

class Worker : public QObject
{
    Q_OBJECT
private slots:
    void on_newPlayer(Tank *t);
    void on_delPlayer(int pos);
    void on_newBullet(Bullet *b);
    void on_delBullet(int pos);
    void on_syncBullet(int pos,int dmg);
    void on_delObjs();
    void on_tbullet();
    void on_tmain();
    void on_tconn();
    void on_tselect();
    //void on_death();
    //void on_kick(); dl
    //void on_fullscreen(); //dl braucht connect mit move
    void on_visible(int v);
    void on_capobj(int num,int owner,int cp);
    void on_setT(int timer);
    void on_pos(Tank *p,int x,int y,int dir,int health, int angle,int spotted,int stimer);
    void on_db(int id, int dmg, int reload, int speed, int health, int width, int height, int barrelLength, double softTerrRes, double hardTerrRes, double treeTerrRes, int treeColl, int vel);
    void on_conn(int id);
    void on_tid();
    void on_spawn(Tank *t);
    void on_disconnect();
    void on_tvisible();
    void on_playerDeath();
    void on_tmove();
private:
    QRect viewRect;
    QTimer *t_bullet;
    QTimer *t_main;
    QTimer *t_conn;
    QTimer *t_id;
    QTimer *t_select;
    QTimer *t_visible;
    QTimer *t_move;
    Tank *ownTank;
    FrmTanks *tankWindow;
    QWidget *mainWindow;
    QVector <Tank*> tanks;
    QVector <Terrain*> lvlObjs;
    QVector <Bullet*> bullets;
    QVector <QRect> spawns;
    QVector <QRect> classRects;
    QVector <dbTank*> dbTanks;
    QVector <int> capObjs;
    int classID;
    Movement *move;
    Network *network;
    Shoot *shoot;
    QPoint *aim;
    QString username;
    double scaleX;
    double scaleY;
    int transX;
    int transY;
    int width;
    int height;
    int timer;
    int startPos;
    int endPos;
    int selected;
    void loadMap();
    int getType(int type);
    bool contains(QString data, QString c);
public:
    explicit Worker(Tank *ownTank,QPoint *aim, int width, int height, QFont f, QWidget *mainWindow,QObject *parent = 0);
    ~Worker();
    void keyP(QKeyEvent *e);
    void keyR(QKeyEvent *e);
    void mPrs(QMouseEvent *e, bool SD);
    void mRls(QMouseEvent *e);
    void notActive();
    void setViewRect(QRect viewRect, int startPos = 0, int endPos = 1200);
    void connectToServer(QString username,QString password,QString version);
    void doSpawn();
    void setScale(double scaleX, double scaleY,int transX,int transY);
    void chat(QString message);
    void close();
    int getDifference(int v1,int v2);
    int getSelected();
    int getClassID();
    double getDifference(double v1, double v2);
    QVector<QRect> getSpawns();
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
    void wrongData(int id);
    void shot();
    void hit(Tank *t, int dmg);
    void spawn();
    void death();
    void msgbox(QString title,QString text);
    void otherDeath(QRect rect);
    void chatS(QString message);
};

#endif // WORKER_H
