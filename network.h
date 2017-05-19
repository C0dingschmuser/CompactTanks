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
    void on_tmain();
private:
    QHostAddress ip;
    QTimer *t_main;
    QTcpSocket *tcpSocket;
    QUdpSocket *udpSocketListen;
    QUdpSocket *udpSocket;
    QVector <Tank*> players;
    Tank *ownTank;
    Tank *sucheTank(QString name);
    int getArrayPos(QString name);
    void fetchUDP(QString data);
    void fetchTCP(QString data);
    bool check(QStringList l,int anz);
    quint16 udpPort;
public:
    explicit Network(Tank *ownTank, QVector<Tank*> t, QHostAddress ip, QObject *parent = 0);
    ~Network();
    void send(QString data);
    int getDistance(QPoint p1,QPoint p2);

signals:
    void newPlayer(Tank *t);
    void newlvlObj(int x,int y,int w,int h,int type);
    void delPlayer(int i);
    void newBullet(Bullet *b);
    void delBullet(int pos);
    void syncBullet(int pos,int x,int y, int elapsed);
    void setViewRange(int vr);
    void delObjs();
    void disconnect();
    void playerDeath();
    void message(QString messageText,int length);
    void killMessage(QString messageText);
    void kick();
public slots:
};

#endif // NETWORK_H
