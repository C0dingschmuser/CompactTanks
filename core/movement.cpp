#include "movement.h"

Movement::Movement(Tank *t, int width, int height, QObject *parent) : QObject(parent)
{
    this->width = width;
    this->height = height;
    this->map = QRectF(0,0,width,height);
    ownTank = t;
    int first = 0;
    sPos = 0;
    ePos = 1200;
    t_move = new QTimer();
    t_moveW = new QTimer();
    t_moveA = new QTimer();
    t_moveS = new QTimer();
    t_moveD = new QTimer();
    connect(t_move,SIGNAL(timeout()),this,SLOT(on_tmove()));
    connect(t_moveW,SIGNAL(timeout()),this,SLOT(on_tmoveW()));
    connect(t_moveA,SIGNAL(timeout()),this,SLOT(on_tmoveA()));
    connect(t_moveS,SIGNAL(timeout()),this,SLOT(on_tmoveS()));
    connect(t_moveD,SIGNAL(timeout()),this,SLOT(on_tmoveD()));
    t_move->start(50);
}

Movement::~Movement()
{
    delete t_moveW;
    delete t_moveA;
    delete t_moveS;
    delete t_moveD;
}

void Movement::run(QThread *thread)
{
    t_move->moveToThread(thread);
    t_moveW->moveToThread(thread);
    t_moveA->moveToThread(thread);
    t_moveS->moveToThread(thread);
    t_moveD->moveToThread(thread);
}

void Movement::on_tmove()
{
    if(!t_moveW->isActive()&&!t_moveA->isActive()&&
            !t_moveS->isActive()&&!t_moveD->isActive()) {
        ownTank->setMoved(false);
        ownTank->setDir(0);
    } else {
        if(t_moveW->isActive()) {
            ownTank->setDir(1);
        } else if(t_moveS->isActive()) {
            ownTank->setDir(2);
        }
    }
    if(!ownTank->isSpawned()) {
        first = 0;
        t_moveW->stop();
        t_moveA->stop();
        t_moveS->stop();
        t_moveD->stop();
    }
}

void Movement::on_tmoveW() {
    if(first==3) return;
    bool ok=true;
    QPolygonF pol = ownTank->getPolygon(1);
    if(ownTank->getVehicleID()!=1) {
        QRectF bRect = pol.boundingRect();
        if(bRect.x()<0||bRect.x()+bRect.width()>map.width()||bRect.y()<0||bRect.y()+bRect.height()>map.height()) {
            //ok = false;
        }
        int min,max;
        if(ownTank->getTeam()==1) {
            min = 2;
            max = 4;
        } else if(ownTank->getTeam()==2) {
            min = 0;
            max = 2;
        }
    }
    if(ok) {
        ownTank->move();
    }
}

void Movement::on_tmoveA() {
    bool ok=true;
    QPolygonF pol = ownTank->getPolygon(4);
    if(ownTank->getVehicleID()!=1) {
        QRectF bRect = pol.boundingRect();
        if(bRect.x()<0||bRect.x()+bRect.width()>map.width()||bRect.y()<0||bRect.y()+bRect.height()>map.height()) {
            //ok = false;
        }
    }
    if(ok) {
        ownTank->getBody()->SetAngularVelocity(-3);
        //ownTank->setTurnAngle(qRadiansToDegrees((double)ownTank->getBody()->GetAngle())-1);
    }
}

void Movement::on_tmoveS() {
    if(first==1) return;
    bool ok=true;
    QPolygonF pol = ownTank->getPolygon(2);
    if(ownTank->getVehicleID()!=1) {
        QRectF bRect = pol.boundingRect();
        if(bRect.x()<0||bRect.x()+bRect.width()>map.width()||bRect.y()<0||bRect.y()+bRect.height()>map.height()) {
            //ok = false;
        }
    }
    if(ok) {
        ownTank->move(1);
    }
}

void Movement::on_tmoveD() {
    bool ok=true;
    QPolygonF pol = ownTank->getPolygon(3);
    if(ownTank->getVehicleID()!=1) {
        QRectF bRect = pol.boundingRect();
        if(bRect.x()<0||bRect.x()+bRect.width()>map.width()||bRect.y()<0||bRect.y()+bRect.height()>map.height()) {
            //ok = false;
        }

    }
    if(ok) {
        ownTank->getBody()->SetAngularVelocity(3);
        //ownTank->setTurnAngle(qRadiansToDegrees((double)ownTank->getBody()->GetAngle())+1);
    }
}

void Movement::keyPressEvent(QVector<Terrain *> lvlObjs, int sPos, int ePos,int key)
{
    if(!ownTank->isSpawned()) return;
    this->lvlObjs = lvlObjs;
    int s = ownTank->getSpeed();
    int t = 2;//ownTank->getTimer();
    this->sPos = sPos;
    this->ePos = ePos;
    if(key==Qt::Key_W) {
        first = 1;
        t_moveW->start(t);
    }
    if(key==Qt::Key_A) {
        first = 2;
        t_moveA->start(t*2);
    }
    if(key==Qt::Key_S&&ownTank->getVehicleID()!=1) {
        first = 3;
        t_moveS->start(t);
    }
    if(key==Qt::Key_D) {
        first = 4;
        t_moveD->start(t*2);
    }
    if(key==Qt::Key_Tab) {
        emit tab();
    }
}

void Movement::keyReleaseEvent(int key)
{
    if(key==Qt::Key_F11) {
        emit fullscreen();
    }
    if(!ownTank->isSpawned()) return;
    if(key==Qt::Key_W&&ownTank->getVehicleID()!=1) {
        t_moveW->stop();
        ownTank->setMoved(false);
    }
    if(key==Qt::Key_A) {
        t_moveA->stop();
        ownTank->setMoved(false);
    }
    if(key==Qt::Key_S) {
        t_moveS->stop();
        ownTank->setMoved(false);
    }
    if(key==Qt::Key_D) {
        t_moveD->stop();
        ownTank->setMoved(false);
    }
    if(key==Qt::Key_Tab) {
        emit tab();
    }
}

void Movement::stop()
{
    if(ownTank->getVehicleID()) return;
    t_moveW->stop();
    t_moveA->stop();
    t_moveS->stop();
    t_moveD->stop();
}

void Movement::setSpawns(QVector<QRect> spawns)
{
    this->spawns = spawns;
}
