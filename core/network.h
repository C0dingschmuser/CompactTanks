#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QTimer>
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
private:
    QHostAddress ip;
    QTimer *t_main;
    QTimer *t_disconnect;
    QTcpSocket *tcpSocket;
    QUdpSocket *udpSocketListen;
    QUdpSocket *udpSocket;
    QVector <Tank*> players;
    QByteArray buffer;
    Tank *ownTank;
    Tank *sucheTank(QString name);
    int getArrayPos(QString name);
    void fetchUDP(QString data);
    void fetchTCP(QString data);
    bool check(QStringList l,int anz);
    quint16 udpPort;
    int timer;
public:
    explicit Network(Tank *ownTank, QVector<Tank*> t, QHostAddress ip, QObject *parent = 0);
    ~Network();
    bool connectToServer(QString username,QString password);
    void send(QString data);
    void sendPos();
    void setTimer(int timer);
    int getDistance(QPoint p1,QPoint p2);

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
    void conn(bool success);
    void hit(Tank *t,int dmg);
    void spawn();
public slots:
};

#endif // NETWORK_H
