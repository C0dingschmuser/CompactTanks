#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QTimer>
#include "tank.h"

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
    quint16 udpPort;
public:
    explicit Network(Tank *ownTank, QVector<Tank*> t, QHostAddress ip, QObject *parent = 0);
    ~Network();

signals:
    void newPlayer(Tank *t);
    void delPlayer(int i);
public slots:
};

#endif // NETWORK_H
