#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QTimer>
#include <QStringList>
#include "tank.h"
#include "bullet.h"

class Network : public QObject
{
    Q_OBJECT
private slots:
    void on_tcpRecv();
    void on_udpRecv();
    void on_disconnect();
    void on_tmain();
    void on_tdisconnect();
    void on_tping();
private:
    QHostAddress ip;
    QTimer *t_main;
    QTimer *t_ping;
    QTimer *t_disconnect;
    QTcpSocket *tcpSocket;
    QUdpSocket *udpSocketListen;
    QUdpSocket *udpSocket;
    QVector <Tank*> players;
    QByteArray buffer;
    Tank *ownTank;
    int getArrayPos(QString name);
    void fetchUDP(QString data);
    void fetchTCP(QString data);
    bool check(QStringList l,int anz);
    quint16 udpPort;
    int timer;
    bool connected;
public:
    explicit Network(Tank *ownTank, QVector<Tank*> t, QHostAddress ip, QObject *parent = 0);
    ~Network();
    bool connectToServer(QString username, QString password, QString version);
    void send(QString data);
    void sendPos();
    void setTimer(int timer);
    void stop();
    int getDistance(QPoint p1,QPoint p2);
    Tank *sucheTank(QString name);

signals:
    void pos(Tank *p,int x,int y,int dir,int health, int angle,int spotted,int stimer);
    void newPlayer(Tank *t);
    void newlvlObj(int x,int y,int w,int h,int type);
    void delPlayer(int i);
    void newBullet(Bullet *b);
    void delBullet(int pos);
    void syncBullet(int pos,int dmg);
    void setViewRange(int vr);
    void delObjs();
    void disconnect();
    void playerDeath();
    void message(QString messageText,int length);
    void killMessage(QString messageText);
    void kick();
    void visible(int visible);
    void capobj(int num,int owner,int cp);
    void setT(int timer);
    void conn(int id);
    void hit(Tank *t,QString dmg);
    void spawn();
    void stats(int id, int dmg, int reload, int speed, int health, int width, int height, int barrelLength,
               double softTerrRes, double hardTerrRes, double treeTerrRes, int treeColl, int vel,
               int camo, int viewrange);
    void spawn(Tank *t);
    void otherDeath(QRect rect);
    void chat(QString message);
    void ping(int ping);
    void teamCP(int team1cp, int team2cp);
    void reset(int team);
    void ownHit();
    void changelog(int size);
public slots:
};

#endif // NETWORK_H
