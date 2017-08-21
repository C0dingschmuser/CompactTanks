#include "network.h"

Network::Network(Tank *ownTank, QVector<Tank *> t, QHostAddress ip,QObject *parent) : QObject(parent)
{
    this->ownTank = ownTank;
    this->players = t;
    this->ip = ip;
    t_main = new QTimer();
    t_disconnect = new QTimer();
    t_ping = new QTimer();
    tcpSocket = new QTcpSocket();
    udpSocket = new QUdpSocket();
    udpSocketListen = new QUdpSocket();
    connected = false;
    //udpSocketListen->bind(QHostAddress::AnyIPv4,8889,QUdpSocket::ShareAddress); //client wartet bei 8889 server bei 8890
    //udpSocketListen->joinMulticastGroup(ip);
    connect(udpSocketListen,SIGNAL(readyRead()),this,SLOT(on_udpRecv()));
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(on_tcpRecv()));
    connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(on_disconnect()));
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(on_disconnect()));
    connect(t_main,SIGNAL(timeout()),this,SLOT(on_tmain()));
    connect(t_disconnect,SIGNAL(timeout()),this,SLOT(on_tdisconnect()));
    connect(t_ping,SIGNAL(timeout()),this,SLOT(on_tping()));
    t_main->start(10);
}

Network::~Network()
{
    tcpSocket->disconnectFromHost();
    delete udpSocket;
    delete udpSocketListen;
    delete tcpSocket;
}

bool Network::connectToServer(QString username, QString password, QString version)
{
    bool ok = false;
    tcpSocket->connectToHost(ip,38888);
    tcpSocket->waitForConnected(1000);
    if(tcpSocket->state()==QTcpSocket::ConnectedState) {
        ok = true;
        ownTank->setName(username);
        send("|0#"+username+"#"+password+"#"+version+"#~");
    }
    return ok;
}

void Network::stop()
{
    t_disconnect->stop();
    t_main->stop();
}

void Network::on_tcpRecv()
{
    buffer += tcpSocket->readAll();
    if(buffer.size()>54) {
        QString input = buffer;
        buffer.clear();
        if(input.contains('|')&&input.at(input.size()-1)=='~') {
            QStringList p = input.split('|');
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
    if(!ownTank->isSpawned()) return;
    QByteArray data;
    data.append(QString("|0#"+ownTank->toString()+QString::number(timer)+"#~").toUtf8());
    udpSocket->writeDatagram(data,ip,38889);
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
    data.insert(1,".");
    tcpSocket->write(data.toLatin1());
}

void Network::setTimer(int timer)
{
    this->timer = timer;
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

void Network::on_tping()
{
    send("|-2#"+QString::number(timer)+"#~");
}

void Network::fetchTCP(QString data)
{
    //qDebug()<<data;
    if(data.at(0)==".") {
        data.remove(0,1);
    } else {
        return;
    }
    QStringList list = data.split("#");
    if(list.at(0)!="") {
        int m = list.at(0).toInt();
        if(list.size()>1) {
            if(list.at(1)!=ownTank->getName()) {
                switch(m) {
                    case -8: //login erfolgreich?
                        {
                            int id = list.at(1).toInt();
                            if(id==1) {
                                connected = true;
                                t_ping->start(1000);
                            }
                            emit conn(id);
                            break;
                        }
                    case -7: //setownpos spawn
                        //emit spawn, start animation
                        if(!ownTank->isSpawned()) {
                            ownTank->setAll(list.at(2).toInt(),list.at(3).toInt());
                            if(list.at(1).toInt()) {
                                ownTank->setColor(1);
                                emit spawn();
                            }
                        }
                    break;
                    case -6: //settimer
                        emit setT(list.at(1).toInt());
                    break;
                    case -5: //setteam
                        if(list.size()>0) {
                            ownTank->setTeam(list.at(1).toInt());
                        }
                    break;
                    case -4: //kick
                        t_main->stop();
                        emit kick();
                    break;
                    case -3: //pos
                        {
                            if(list.size()>8) {
                                //9 = timer
                                Tank *tmp = sucheTank(list.at(1));
                                emit pos(tmp,list.at(2).toInt(),list.at(3).toInt(),list.at(4).toInt(),list.at(6).toInt(),list.at(8).toInt(),list.at(5).toInt(),list.at(9).toInt());
                                /*tmp->setAll(list.at(2).toInt(),list.at(3).toInt(),list.at(4).toInt(),list.at(6).toInt());
                                tmp->setAngle(list.at(8).toInt());
                                tmp->setSpotted(list.at(5).toInt());*/
                            }
                            break;
                        }
                    case -2: //bulletsync
                        emit syncBullet(list.at(1).toInt(),list.at(2).toInt());
                    break;
                    case -1: //health
                        if(list.size()>0) {
                            ownTank->setHealth(list.at(1).toInt());
                        }
                    break;
                    case 0: //farbe setzen
                        if(list.size()>2) {
                            ownTank->setColor(1);
                            ownTank->teleport(list.at(2).toInt(),list.at(3).toInt());
                            t_main->start(10);
                        }
                    break;
                    case 1: //spieler hinzufügen
                        {
                            if(list.size()>4) {
                                Tank *t = new Tank(QRect(-200,-200,40,40),list.at(1),list.at(7).toInt());
                                if(t->getTeam()==ownTank->getTeam()) {
                                    t->setColor(1);
                                } else {
                                    t->setColor(0);
                                }
                                players.append(t);
                                emit newPlayer(t);
                                emit killMessage(list.at(1)+" joined");
                            }
                            break;
                        }
                    case 2: //spieler entfernen
                        {
                            if(list.size()>0) {
                                Tank *t = sucheTank(list.at(1));
                                int pos = getArrayPos(t->getName());
                                emit killMessage(t->getName()+" left");
                                players.removeAt(pos);
                                emit delPlayer(pos);
                                t->deleteLater();
                            }
                        }
                    break;
                    case 3: //lvlobj
                        {
                            if(list.size()>4) {
                                //emit newlvlObj(list.at(1).toInt(),list.at(2).toInt(),list.at(3).toInt(),list.at(4).toInt(),list.at(5).toInt());
                            }
                        }
                    break;
                    case 4: //add bullet
                        {
                            if(list.size()>5) {
                                Bullet *b = new Bullet(list.at(1).toInt(),list.at(2).toInt(),list.at(3).toDouble(),list.at(4).toDouble(),list.at(5).toInt(),list.at(7));
                                b->setDmg(list.at(6).toInt());
                                bool enabled = true;
                                if(!ownTank->getRect().intersects(b->get())) {
                                    //enabled = false;
                                }
                                b->setEnabled(enabled);
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
                        if(list.size()>3) {
                            ownTank->setSpawned(false);
                            ownTank->setDeathPoint(QPoint(ownTank->getRect().x(),ownTank->getRect().y()));
                            ownTank->teleport(list.at(2).toInt(),list.at(3).toInt());
                            emit killMessage(list.at(1)+" killed "+ownTank->getName());
                            emit playerDeath();
                        }
                    break;
                    case 8: //otherdeath
                        if(list.size()>2) {
                            //qDebug()<<list.at(1);
                            Tank *tmp = sucheTank(list.at(1));
                            QRect rect = tmp->getRect();
                            rect.moveTo(list.at(2).toInt(),list.at(3).toInt());
                            emit otherDeath(rect);
                            tmp->setSpawned(false);
                            tmp->teleport(-200,-200);
                            emit killMessage(list.at(4)+" killed "+list.at(1));
                        }
                    break;
                    case 9: //message
                        emit message(list.at(1),list.at(2).toInt());
                    break;
                    case 10: //visible + health + coins
                        //qDebug()<<list;
                        ownTank->setHealth(list.at(2).toInt());
                        ownTank->setCoins(list.at(3).toInt());
                        emit visible(list.at(1).toInt());
                    break;
                    case 11: //capobj
                        emit capobj(list.at(1).toInt(),list.at(2).toInt(),list.at(3).toInt());
                    break;
                    case 12: //hit
                        {
                            Tank *t = sucheTank(list.at(1));
                            QString v = "-";
                            if(list.at(3).toInt()) v = "+";
                            emit hit(t,v+list.at(2));
                        }
                    break;
                    case 13: //spawn other
                        {
                            Tank *t = sucheTank(list.at(1));
                            t->teleport(list.at(2).toInt(),list.at(3).toInt());
                            t->setType(list.at(4).toInt());
                            //t->setData(list.at(4).toInt(),list.at(5).toInt(),list.at(6).toInt(),list.at(7).toInt(),0,list.at(8).toInt(),list.at(9).toInt(),0,0);
                            t->setSpawned(true);
                            emit spawn(t);
                        }
                    break;
                    case 14: //stats
                        emit stats(list.at(1).toInt(),list.at(2).toInt(),list.at(3).toInt(),list.at(4).toInt(),list.at(5).toInt(),list.at(6).toInt(),
                                   list.at(7).toInt(),list.at(8).toInt(),list.at(9).toDouble(),list.at(10).toDouble(),list.at(11).toDouble(),list.at(12).toInt(),
                                   list.at(13).toInt(),list.at(14).toInt(),list.at(15).toInt());
                        break;
                    case 15: //chat
                        emit chat(list.at(1));
                    break;
                    case 16: //teamwechsel
                        if(list.at(1)==ownTank->getName()) {
                            if(ownTank->getTeam()==1) {
                                ownTank->setTeam(2);
                            } else if(ownTank->getTeam()==2) {
                                ownTank->setTeam(2);
                            }
                        } else {
                            Tank *t = sucheTank(list.at(1));
                            if(t->getTeam()==1) {
                                t->setTeam(2);
                            } else if(t->getTeam()==2) {
                                t->setTeam(1);
                            }
                        }
                    break;
                    case 17: //ping
                        emit ping(list.at(1).toInt());
                    break;
                    case 18: //teamCP
                        emit teamCP(list.at(1).toInt(),list.at(2).toInt());
                    break;
                    case 19: //win/lose
                        emit reset(list.at(1).toInt());
                    break;
                    case 20: //ownhit
                        emit ownHit();
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
                        //tmp->setAll(list.at(3).toInt(),list.at(4).toInt(),list.at(5).toInt());
                    }
                }
            break;
            case 1: //bulletsync
                emit syncBullet(list.at(1).toInt(),list.at(2).toInt());
            break;
            case 2: //viewRange
                //ownTank->setViewRange(list.at(2).toInt());
            break;
        }
    }
}

Tank* Network::sucheTank(QString name)
{
    Tank *tmp = NULL;
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
