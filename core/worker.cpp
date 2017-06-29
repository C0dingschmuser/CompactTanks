#include "worker.h"

Worker::Worker(Tank *ownTank,QPoint *aim,int width,int height,QObject *parent) : QObject(parent)
{
    this->width = width;
    this->height = height;
    this->ownTank = ownTank;
    this->aim = aim;
    t_bullet = new QTimer(this);
    t_main = new QTimer(this);
    t_conn = new QTimer(this);
    t_main->setTimerType(Qt::PreciseTimer);
    sound = new Sound(this);
    move = new Movement(this->ownTank,this->width,this->height);
    network = new Network(this->ownTank,tanks,QHostAddress("127.0.0.1")); //ändern
    shoot = new Shoot(this->ownTank,network,this->aim,sound);
    connect(t_bullet,SIGNAL(timeout()),this,SLOT(on_tbullet()));
    connect(network,SIGNAL(newPlayer(Tank*)),this,SLOT(on_newPlayer(Tank*)));
    connect(network,SIGNAL(delPlayer(int)),this,SLOT(on_delPlayer(int)));
    connect(network,SIGNAL(newlvlObj(int,int,int,int,int)),this,SLOT(on_newlvlObj(int,int,int,int,int)));
    connect(network,SIGNAL(delObjs()),this,SLOT(on_delObjs()));
    connect(network,SIGNAL(newBullet(Bullet*)),this,SLOT(on_newBullet(Bullet*)));
    connect(network,SIGNAL(delBullet(int)),this,SLOT(on_delBullet(int)));
    connect(network,SIGNAL(syncBullet(int,int,int,int)),this,SLOT(on_syncBullet(int,int,int,int)));
    connect(network,SIGNAL(visible(int)),this,SLOT(on_visible(int)));
    connect(network,SIGNAL(message(QString,int)),this,SIGNAL(message(QString,int)));
    connect(network,SIGNAL(killMessage(QString)),this,SIGNAL(killMessage(QString)));
    connect(network,SIGNAL(kick()),this,SIGNAL(kick()));
    connect(network,SIGNAL(capobj(int,int,int)),this,SLOT(on_capobj(int,int,int)));
    connect(network,SIGNAL(setT(int)),this,SLOT(on_setT(int)));
    connect(network,SIGNAL(pos(Tank*,int,int,int,int,int,int,int)),this,SLOT(on_pos(Tank*,int,int,int,int,int,int,int)));
    connect(network,SIGNAL(conn(bool)),this,SLOT(on_conn(bool)));
    connect(move,SIGNAL(fullscreen()),this,SIGNAL(fullscreen()));
    connect(move,SIGNAL(tab()),this,SIGNAL(tab()));
    connect(shoot,SIGNAL(newBullet(Bullet*)),this,SLOT(on_newBullet(Bullet*)));
    connect(t_main,SIGNAL(timeout()),this,SLOT(on_tmain()));
    connect(t_conn,SIGNAL(timeout()),this,SLOT(on_tconn()));
    sound->setVolume(0.1);
    t_bullet->start(2);
    t_main->start(4);
    //loadMap();
}

Worker::~Worker()
{
    disconnect(network,SIGNAL(disconnect()),this,SIGNAL(disconnected()));
    delete move;
    delete network;
}

void Worker::connectToServer(QString username, QString password)
{
    //t_conn->start(200);
    if(network->connectToServer(username,password)) {
        this->username = username;
        t_conn->stop();
    } else {
        emit connFail();
    }
}

void Worker::on_conn(bool success)
{
    if(success) {
        connect(network,SIGNAL(disconnect()),this,SIGNAL(disconnected()));
        ownTank->setName(username);
        emit connSuccess();
    } else {
        emit wrongData();
    }
}

void Worker::on_pos(Tank *p, int x, int y, int dir, int health, int angle, int spotted, int stimer)
{
    int diff = getDifference(stimer,timer);
    p->setAll(x,y,dir,health,diff/8);
    p->setAngle(angle);
    p->setSpotted(spotted);
    /*qDebug()<<"--------";
    qDebug()<<timer;
    qDebug()<<stimer;*/
}

void Worker::on_tconn()
{
    t_conn->stop();
    //emit connFail();
}

void Worker::on_newPlayer(Tank *t)
{
    tanks.append(t);
    emit newPlayer(t);
}

void Worker::on_delPlayer(int pos)
{
    tanks.removeAt(pos);
    emit delPlayer(pos);
}

void Worker::on_newlvlObj(int x, int y, int w, int h, int type)
{
    loadMap();
}

void Worker::on_delObjs()
{
    for(int i=0;i<lvlObjs.size();i++) {
        delete lvlObjs[i];
    }
    lvlObjs.resize(0);
    emit delObjs();
}

void Worker::on_newBullet(Bullet *b)
{
    bullets.append(b);
    emit newBullet(b);
}

void Worker::on_delBullet(int pos)
{
    if(bullets.size()-1>=pos) {
        bullets.removeAt(pos);
        emit delBullet(pos);
    }
}

void Worker::on_syncBullet(int pos, int x, int y, int elapsed)
{
    //keine weiterleitung da pointer
    if(bullets.size()-1>=pos) {
        if(!bullets[pos]->getEnabled()) {
            if(viewRect.intersects(QRect(x,y,10,10))) {
                bullets[pos]->setEnabled(true);
            }
        }
        bullets[pos]->sync(x,y,elapsed);
    }
}

void Worker::on_tbullet()
{
    if(bullets.size()>0) {
        for(int i=0;i<bullets.size();i++) {
            if(bullets[i]->getEnabled()) {
                bullets[i]->update();
            }
            for(int a=0;a<tanks.size();a++) {
                if(bullets[i]->getShooter()!=tanks[a]->getName()) {
                    if(bullets[i]->get().intersects(tanks[a]->getRect())) {
                        bullets[i]->setEnabled(false);
                    }
                }
            }
        }
    }
}

void Worker::on_visible(int v)
{
    bool ok=false;
    for(int i=0;i<lvlObjs.size();i++) {
        if(ownTank->getRect().intersects(lvlObjs.at(i)->getRect())) {
            if(lvlObjs[i]->getType()>0&&lvlObjs[i]->getType()<3) {
                ok = true;
                break;
            }
        }
    }
    if(v||ok) {
        ownTank->setVisible(true);
        ok = true;
    } else {
        ok = false;
        ownTank->setVisible(false);
    }
    for(int i=0;i<tanks.size();i++) {
        bool ok=false;
        for(int a=0;a<lvlObjs.size();a++) {
            if(tanks[i]->getTeam()==ownTank->getTeam()) {
                if(tanks[i]->getRect().intersects(lvlObjs[a]->getRect())&&
                        lvlObjs[a]->getType()>0&&lvlObjs[a]->getType()<3) {
                    ok = true;
                    break;
                }
            } else {
                ok = true;
            }
        }
        if(ok||tanks[i]->getSpotted()) {
            tanks[i]->setVisible(true);
            ok = true;
        } else {
            ok = false;
            tanks[i]->setVisible(false);
        }
    }
    emit visible(ok);
}

void Worker::on_capobj(int num, int owner, int cp)
{
    lvlObjs[num]->setOwner(owner);
    lvlObjs[num]->setAmount(cp);
}

void Worker::on_setT(int timer)
{
    this->timer = timer;
}

void Worker::on_tmain()
{
    timer+= 4;
    if(timer>5000) {
        timer = 0;
    }
    for(int i=0;i<tanks.size();i++) {
        if(tanks[i]->getRect().intersects(viewRect)&&tanks[i]->getRect().x()>0) {
            tanks[i]->move();
        } else {
            tanks[i]->teleport(-200,-200);
        }
    }
}

void Worker::loadMap()
{
    QFile file(":/map/map.dat");
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString line;
    while(!in.atEnd()) {
        line += in.readLine();
    }
    QStringList basic = line.split(",");
    int max2 = width/72;
    int max1 = height/72;
    for(int i=0;i<max1;i++) {
        for(int a=0;a<max2;a++) {
            Terrain *obj = new Terrain(0+(72*a),0+(72*i),72,72,getType(basic.at(lvlObjs.size()).toInt()),getType(basic.at(lvlObjs.size()+(max1*max2)).toInt()));
            lvlObjs.append(obj);
        }
    }
    for(int i=0;i<lvlObjs.size();i++) {
        emit newlvlObj(lvlObjs[i]);
    }
    //emit newMap(lvlObjs);
}

int Worker::getType(int type) {
    int t = type-1;
    return t;
}

int Worker::getDifference(int v1, int v2)
{
    int diff = 0;
    if(v1>v2) {
        diff = v1-v2;
    } else if(v2>v1) {
        diff = v2-v1;
    }
    return diff;
}

void Worker::keyP(QKeyEvent *e)
{
    move->keyPressEvent(e,lvlObjs);
}

void Worker::keyR(QKeyEvent *e)
{
    move->keyReleaseEvent(e);
}

void Worker::mPrs(QMouseEvent *e)
{
    shoot->MousePressEvent(e);
}

void Worker::mRls(QMouseEvent *e)
{
    shoot->MouseReleaseEvent(e);
}

void Worker::notActive()
{
    move->stop();
}

void Worker::setViewRect(QRect viewRect)
{
    this->viewRect = viewRect;
}
