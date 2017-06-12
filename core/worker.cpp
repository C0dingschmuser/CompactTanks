#include "worker.h"

Worker::Worker(Tank *ownTank,QPoint *aim,int width,int height,QObject *parent) : QObject(parent)
{
    this->width = width;
    this->height = height;
    this->ownTank = ownTank;
    this->aim = aim;
    t_bullet = new QTimer(this);
    move = new Movement(this->ownTank,this->width,this->height);
    network = new Network(this->ownTank,tanks,QHostAddress("94.114.254.180"));
    shoot = new Shoot(this->ownTank,network,this->aim);
    connect(t_bullet,SIGNAL(timeout()),this,SLOT(on_tbullet()));
    connect(network,SIGNAL(newPlayer(Tank*)),this,SLOT(on_newPlayer(Tank*)));
    connect(network,SIGNAL(delPlayer(int)),this,SLOT(on_delPlayer(int)));
    connect(network,SIGNAL(newlvlObj(int,int,int,int,int)),this,SLOT(on_newlvlObj(int,int,int,int,int)));
    connect(network,SIGNAL(delObjs()),this,SLOT(on_delObjs()));
    connect(network,SIGNAL(newBullet(Bullet*)),this,SLOT(on_newBullet(Bullet*)));
    connect(network,SIGNAL(delBullet(int)),this,SLOT(on_delBullet(int)));
    connect(network,SIGNAL(syncBullet(int,int,int,int)),this,SLOT(on_syncBullet(int,int,int,int)));
    connect(network,SIGNAL(visible(int)),this,SLOT(on_visible(int)));
    connect(network,SIGNAL(disconnect()),this,SIGNAL(disconnected()));
    connect(network,SIGNAL(message(QString,int)),this,SIGNAL(message(QString,int)));
    connect(network,SIGNAL(killMessage(QString)),this,SIGNAL(killMessage(QString)));
    connect(network,SIGNAL(kick()),this,SIGNAL(kick()));
    connect(network,SIGNAL(capobj(int,int,int)),this,SLOT(on_capobj(int,int,int)));
    connect(move,SIGNAL(fullscreen()),this,SIGNAL(fullscreen()));
    connect(move,SIGNAL(tab()),this,SIGNAL(tab()));
    connect(shoot,SIGNAL(newBullet(Bullet*)),this,SLOT(on_newBullet(Bullet*)));
    //loadMap();
    if(!network->connectToServer()) {
        emit connFail();
    }
    t_bullet->start(2);
}

Worker::~Worker()
{
    disconnect(network,SIGNAL(disconnect()),this,SIGNAL(disconnected()));
    delete move;
    delete network;
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
        if(ownTank->getRect().intersects(lvlObjs[i]->getRect())&&
                lvlObjs[i]->getType()>0&&lvlObjs[i]->getType()<3) {
            ok = true;
            break;
        }
    }
    if(v||ok) {
        ok = true;
    } else {
        ok = false;
    }
    emit visible(ok);
}

void Worker::on_capobj(int num, int owner, int cp)
{
    lvlObjs[num]->setOwner(owner);
    lvlObjs[num]->setAmount(cp);
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
            Terrain *obj = new Terrain(0+(72*a),0+(72*i),72,72,getType(basic.at(lvlObjs.size()).toInt()));
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

void Worker::keyP(QKeyEvent *e)
{
    move->keyPressEvent(e);
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
