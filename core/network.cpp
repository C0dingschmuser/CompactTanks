#include "network.h"

Network::Network(Tank *ownTank, QVector<Tank *> t, QHostAddress ip,QObject *parent) : QObject(parent)
{
    this->ownTank = ownTank;
    this->players = t;
    this->ip = ip;
    t_main = new QTimer(this);
    t_disconnect = new QTimer(this);
    tcpSocket = new QTcpSocket(this);
    udpSocket = new QUdpSocket(this);
    udpSocketListen = new QUdpSocket(this);
    //udpSocketListen->bind(QHostAddress::AnyIPv4,8889,QUdpSocket::ShareAddress); //client wartet bei 8889 server bei 8890
    //udpSocketListen->joinMulticastGroup(ip);
    connect(udpSocketListen,SIGNAL(readyRead()),this,SLOT(on_udpRecv()));
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(on_tcpRecv()));
    connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(on_disconnect()));
    connect(t_main,SIGNAL(timeout()),this,SLOT(on_tmain()));
    connect(t_disconnect,SIGNAL(timeout()),this,SLOT(on_tdisconnect()));
    //t_main->start(10);
}

Network::~Network()
{
    tcpSocket->disconnectFromHost();
    delete udpSocket;
    delete udpSocketListen;
    delete tcpSocket;
}

bool Network::connectToServer()
{
    bool ok;
    tcpSocket->connectToHost(ip,8888);
    ok = tcpSocket->waitForConnected(3000);
    if(ok) {
        send("|0#"+ownTank->getName()+"#");
    }
    return ok;
}

void Network::on_tcpRecv()
{
    buffer += tcpSocket->readAll();
    if(buffer.size()>29) {
        QString input = buffer;
        buffer.clear();
        if(input.contains("|")) {
            QStringList p = input.split("|");
            for(int i=1;i<p.size();i++) {
                fetchTCP(p.at(i));
            }
        } else {
            fetchTCP(input);
        }
    }
}

void Network::on_disconnect()
{
    t_disconnect->start(200);
}

void Network::on_tdisconnect()
{
    emit disconnect();
    t_disconnect->stop();
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
    udpSocket->writeDatagram(data,ip,8889);
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

void Network::send(QString data)
{
    tcpSocket->write(data.toLatin1());
    tcpSocket->flush();
}

int Network::getDistance(QPoint p1, QPoint p2)
{
    return abs((int)sqrt(pow(p1.x()-p2.x(),2)+pow(p1.y()-p2.y(),2)));
}

bool Network::check(QStringList l, int anz)
{
    //spaeter
    Q_UNUSED(l);
    Q_UNUSED(anz);
    return true;
}

void Network::fetchTCP(QString data)
{
    QStringList list = data.split("#");
    if(list.at(0)!="") {
        int m = list.at(0).toInt();
        if(list.size()>1) {
            if(list.at(1)!=ownTank->getName()) {
                switch(m) {
                    case -4: //kick
                        t_main->stop();
                        emit kick();
                    break;
                    case -3: //pos
                        {
                            if(list.size()>4) {
                                Tank *tmp = sucheTank(list.at(1));
                                tmp->setAll(list.at(2).toInt(),list.at(3).toInt(),list.at(4).toInt());
                            }
                        }
                    break;
                    case -2: //bulletsync
                        if(list.size()>4) {
                            emit syncBullet(list.at(1).toInt(),list.at(2).toInt(),list.at(3).toInt(),list.at(4).toInt());
                        }
                    break;
                    case -1: //viewRange
                        if(list.size()>0) {
                            ownTank->setViewRange(list.at(1).toInt());
                        }
                    break;
                    case 0: //farbe setzen
                        if(list.size()>2) {
                            ownTank->setColor(list.at(1).toInt());
                            ownTank->teleport(list.at(2).toInt(),list.at(3).toInt());
                            t_main->start(10);
                        }
                    break;
                    case 1: //spieler hinzufügen
                        {
                            if(list.size()>4) {
                                Tank *t = new Tank(QRect(list.at(2).toInt(),list.at(3).toInt(),40,40),list.at(1));
                                t->setColor(list.at(5).toInt());
                                t->teleport(list.at(2).toInt(),list.at(3).toInt());
                                players.append(t);
                                emit newPlayer(t);
                                emit killMessage(list.at(1)+" joined");
                            }
                        }
                    break;
                    case 2: //spieler entfernen
                        {
                            if(list.size()>0) {
                                Tank *t = sucheTank(list.at(1));
                                int pos = getArrayPos(t->getName());
                                emit killMessage(t->getName()+" left");
                                delete t;
                                players.removeAt(pos);
                                emit delPlayer(pos);
                            }
                        }
                    break;
                    case 3: //lvlobj
                        {
                            if(list.size()>4) {
                                emit newlvlObj(list.at(1).toInt(),list.at(2).toInt(),list.at(3).toInt(),list.at(4).toInt(),list.at(5).toInt());
                            }
                        }
                    break;
                    case 4: //add bullet
                        {
                            if(list.size()>5) {
                                Bullet *b = new Bullet(list.at(1).toInt(),list.at(2).toInt(),list.at(3).toDouble(),list.at(4).toDouble(),list.at(5).toInt(),list.at(6));
                                emit newBullet(b);
                            }
                        }
                    break;
                    case 5: //del bullet
                        {
                            if(list.size()>0) {
                                emit delBullet(list.at(1).toInt());
                            }
                        }
                    break;
                    case 6: //del all objs
                        {
                            emit delObjs();
                        }
                    break;
                    case 7: //ownplayerdeath
                        if(list.size()>2) {
                            ownTank->teleport(list.at(2).toInt(),list.at(3).toInt());
                            emit killMessage(list.at(1)+" killed "+ownTank->getName());
                            emit playerDeath();
                        }
                    break;
                    case 8: //otherdeath
                        if(list.size()>2) {
                            //qDebug()<<list.at(1);
                            Tank *tmp = sucheTank(list.at(1));
                            tmp->teleport(-200,-200);
                            emit killMessage(list.at(4)+" killed "+list.at(1));
                        }
                    break;
                    case 9: //message
                        emit message(list.at(1),list.at(2).toInt());
                    break;
                    case 10: //visible
                        emit visible(list.at(1).toInt());
                    break;
                }
            }
        }
    }
}

void Network::fetchUDP(QString data)
{
    QStringList list = data.split("#"); //max: 11
    if(list.at(1)==ownTank->getName()) {
        int m = list.at(0).toInt();
        switch(m) {
            case 0: //pos
                {
                    Tank *tmp = sucheTank(list.at(2));
                    if(tmp->getRect().x()==-200) {
                        tmp->teleport(list.at(3).toInt(),list.at(4).toInt());
                    } else {
                        tmp->setAll(list.at(3).toInt(),list.at(4).toInt(),list.at(5).toInt());
                    }
                }
            break;
            case 1: //bulletsync
                emit syncBullet(list.at(1).toInt(),list.at(2).toInt(),list.at(3).toInt(),list.at(4).toInt());
            break;
            case 2: //viewRange
                ownTank->setViewRange(list.at(2).toInt());
            break;
        }
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
