#include "network.h"

Network::Network(Tank *ownTank, QVector<Tank *> t, QHostAddress ip,QObject *parent) : QObject(parent)
{
    this->ownTank = ownTank;
    this->players = t;
    this->ip = ip;
    t_main = new QTimer();
    tcpSocket = new QTcpSocket();
    udpSocket = new QUdpSocket();
    udpSocketListen = new QUdpSocket();
    udpSocketListen->bind(QHostAddress::AnyIPv4,8889,QUdpSocket::ShareAddress); //client wartet bei 8889 server bei 8890
    udpSocketListen->joinMulticastGroup(ip);
    tcpSocket->connectToHost(ip,8888);
    tcpSocket->waitForConnected();
    tcpSocket->write(QString("|0#"+ownTank->getName()+"#").toLatin1());
    tcpSocket->flush();
    connect(udpSocketListen,SIGNAL(readyRead()),this,SLOT(on_udpRecv()));
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(on_tcpRecv()));
    connect(t_main,SIGNAL(timeout()),this,SLOT(on_tmain()));
    t_main->start(10);
}

Network::~Network()
{
    delete udpSocket;
    delete udpSocketListen;
    delete tcpSocket;
}

void Network::on_tcpRecv()
{
    QString input = tcpSocket->readAll();
    if(input.contains("|")) {
        QStringList p = input.split("|");
        for(int i=1;i<p.size();i++) {
            fetchTCP(p.at(i));
        }
    } else {
        fetchTCP(input);
    }
}

void Network::on_tmain()
{
    /*
    if(!ownTank->getMoved()) {
        t_main->setInterval(100);
    } else if(ownTank->getMoved()&&t_main->interval()>10){
        t_main->setInterval(10);
    }*/
    QByteArray data;
    data.append(QString("|0#"+ownTank->toString()).toUtf8());
    udpSocket->writeDatagram(data,ip,8890);
}

void Network::on_udpRecv()
{
    //qDebug()<<"recv";
    QByteArray buffer;
    buffer.resize(udpSocketListen->pendingDatagramSize());
    udpSocketListen->readDatagram(buffer.data(),buffer.size());
    QString input = QString::fromUtf8(buffer);
    if(input.contains("|")) {
        QStringList p = input.split("|");
        for(int i=1;i<p.size();i++) {
            fetchUDP(p.at(i));
        }
    } else {
        fetchUDP(input);
    }
}

void Network::fetchTCP(QString data)
{
    QStringList list = data.split("#"); //max: 11
    int m = list.at(0).toInt();
    if(list.at(1)!=ownTank->getName()) {
        switch(m) {
            case 1: //spieler hinzufügen
                {
                    Tank *t = new Tank(QRect(list.at(2).toInt(),list.at(3).toInt(),20,20),list.at(1));
                    players.append(t);
                    emit newPlayer(t);
                }
            break;
            case 2: //spieler entfernen
                {
                    Tank *t = sucheTank(list.at(1));
                    int pos = getArrayPos(t->getName());
                    delete t;
                    players.removeAt(pos);
                    emit delPlayer(pos);
                }
            break;
        }
    }
}

void Network::fetchUDP(QString data)
{
    QStringList list = data.split("#"); //max: 11
    Tank *tmp = sucheTank(list.at(1));
    int m = list.at(0).toInt();
    switch(m) {
        case 0: //pos
            tmp->setAll(list.at(2).toInt(),list.at(3).toInt(),0);
        break;
    }
}

Tank* Network::sucheTank(QString name)
{
    Tank *tmp = new Tank();
    for(int i=0;i<players.size();i++) {
        if(players[i]->getName()==name) {
            tmp = players[i];
            break;
        }
    }
    return tmp;
}

int Network::getArrayPos(QString name)
{
    int c = 0;
    for(int i=0;i<players.size();i++) {
        if(players[i]->getName()==name) {
            c = i;
            break;
        }
    }
    return c;
}
