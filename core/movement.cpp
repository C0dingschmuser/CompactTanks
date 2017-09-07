#include "movement.h"

Movement::Movement(Tank *t, int width, int height, QObject *parent) : QObject(parent)
{
    this->width = width;
    this->height = height;
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
    }
    if(!ownTank->isSpawned()) {
        t_moveW->stop();
        t_moveA->stop();
        t_moveS->stop();
        t_moveD->stop();
    }
}

void Movement::on_tmoveW() {
    QRect r = ownTank->getRect();
    int s = ownTank->getSpeed();
    if(((!t_moveA->isActive()&&!t_moveD->isActive())||first==1)&&!t_moveS->isActive()) {
        if(r.y()>=0+s) {
            bool ok=false;
            if(ownTank->getVehicleID()!=1) {
                for(int i=sPos;i<ePos;i++) {
                    if(QRect(r.x(),r.y()-s,r.width(),r.height()).intersects(lvlObjs[i]->getRect())) {
                        if(!lvlObjs[i]->getType()) {
                            ok = true;
                            break;
                        }
                    }
                }
            }
            int min,max;
            switch(ownTank->getTeam()) {
                case 1:
                    min=2;
                    max=4;
                break;
                case 2:
                    min=0;
                    max=2;
                break;
            }
            for(int i=min;i<max;i++) {
                if(QRect(r.x(),r.y()-s,r.width(),r.height()).intersects(spawns[i])) ok = true;
            }
            if(ownTank->getVehicleID()==1) ok = false;
            if(!ok) {
                ownTank->w();
            }
        } else if(ownTank->getVehicleID()==1) {
            int dir = qrand() %(2-0) + 0;
            t_moveW->stop();
            if(dir) {
                first = 1;
                t_moveA->start(ownTank->getTimer());
            } else {
                first = 4;
                t_moveD->start(ownTank->getTimer());
            }
        }
        ownTank->setMoved(true);
    }
}

void Movement::on_tmoveA() {
    QRect r = ownTank->getRect();
    int s = ownTank->getSpeed();
    if(((!t_moveW->isActive()&&!t_moveS->isActive())||first==2)&&!t_moveD->isActive()) {
        if(r.x()>=0+s) {
            bool ok=false;
            if(ownTank->getVehicleID()!=1) {
                for(int i=sPos;i<ePos;i++) {
                    if(QRect(r.x()-s,r.y(),r.width(),r.height()).intersects(lvlObjs[i]->getRect())) {
                        if(!lvlObjs[i]->getType()) {
                            ok = true;
                            break;
                        }
                    }
                }
            }
            int min,max;
            switch(ownTank->getTeam()) {
                case 1:
                    min=2;
                    max=4;
                break;
                case 2:
                    min=0;
                    max=2;
                break;
            }
            for(int i=min;i<max;i++) {
                if(QRect(r.x()-s,r.y(),r.width(),r.height()).intersects(spawns[i])) ok = true;
            }
            if(ownTank->getVehicleID()==1) ok = false;
            if(!ok) {
                ownTank->a();
            }
        } else if(ownTank->getVehicleID()==1) {
            int dir = qrand() %(2-0) + 0;
            t_moveA->stop();
            if(dir) {
                first = 1;
                t_moveW->start(ownTank->getTimer());
            } else {
                first = 3;
                t_moveS->start(ownTank->getTimer());
            }
        }
        ownTank->setMoved(true);
    }
}

void Movement::on_tmoveS() {
    QRect r = ownTank->getRect();
    int s = ownTank->getSpeed();
    if(((!t_moveA->isActive()&&!t_moveD->isActive())||first==3)&&!t_moveW->isActive()) {
        if(r.bottom()<=height-s*2) {
            bool ok=false;
            if(ownTank->getVehicleID()!=1) {
                for(int i=sPos;i<ePos;i++) {
                    if(QRect(r.x(),r.y()+s,r.width(),r.height()).intersects(lvlObjs[i]->getRect())) {
                        if(!lvlObjs[i]->getType()) {
                            ok = true;
                            break;
                        }
                    }
                }
            }
            int min,max;
            switch(ownTank->getTeam()) {
                case 1:
                    min=2;
                    max=4;
                break;
                case 2:
                    min=0;
                    max=2;
                break;
            }
            for(int i=min;i<max;i++) {
                if(QRect(r.x(),r.y()+s,r.width(),r.height()).intersects(spawns[i])) ok = true;
            }
            if(ownTank->getVehicleID()==1) ok = false;
            if(!ok) {
                ownTank->s();
            }
        } else if(ownTank->getVehicleID()==1) {
            int dir = qrand() %(2-0) + 0;
            t_moveS->stop();
            if(dir) {
                first = 2;
                t_moveA->start(ownTank->getTimer());
            } else {
                first = 4;
                t_moveD->start(ownTank->getTimer());
            }
        }
        ownTank->setMoved(true);
    }
}

void Movement::on_tmoveD() {
    QRect r = ownTank->getRect();
    int s = ownTank->getSpeed();
    if(((!t_moveW->isActive()&&!t_moveS->isActive())||first==4)&&!t_moveA->isActive()) {
        if(r.right()<=width-s*2) {
            bool ok=false;
            if(ownTank->getVehicleID()!=1) {
                for(int i=sPos;i<ePos;i++) {
                    if(QRect(r.x()+s,r.y(),r.width(),r.height()).intersects(lvlObjs[i]->getRect())) {
                        if(!lvlObjs[i]->getType()) {
                            ok = true;
                            break;
                        }
                    }
                }
            }
            int min,max;
            switch(ownTank->getTeam()) {
                case 1:
                    min=2;
                    max=4;
                break;
                case 2:
                    min=0;
                    max=2;
                break;
            }
            for(int i=min;i<max;i++) {
                if(QRect(r.x()+s,r.y(),r.width(),r.height()).intersects(spawns[i])) ok = true;
            }
            if(ownTank->getVehicleID()==1) ok = false;
            if(!ok) {
                ownTank->d();
            }
        } else if(ownTank->getVehicleID()==1) {
            int dir = qrand() %(2-0) + 0;
            t_moveD->stop();
            if(dir) {
                first = 1;
                t_moveW->start(ownTank->getTimer());
            } else {
                first = 3;
                t_moveS->start(ownTank->getTimer());
            }
        }
        ownTank->setMoved(true);
    }
}

void Movement::keyPressEvent(QVector<Terrain *> lvlObjs, int sPos, int ePos,int key)
{
    if(!ownTank->isSpawned()) return;
    this->lvlObjs = lvlObjs;
    QRect r = ownTank->getRect();
    int s = ownTank->getSpeed();
    int t = ownTank->getTimer();
    this->sPos = sPos;
    this->ePos = ePos;
    if(key==Qt::Key_W) {
        if(r.y()>=0+s) {
            first = 1;
            t_moveW->start(t);
            if(ownTank->getVehicleID()==1) {
                t_moveA->stop();
                t_moveS->stop();
                t_moveD->stop();
            }
        }
    }
    if(key==Qt::Key_A) {
        if(r.x()>=0+s) {
            first = 2;
            t_moveA->start(t);
            if(ownTank->getVehicleID()==1) {
                t_moveW->stop();
                t_moveS->stop();
                t_moveD->stop();
            }
        }
    }
    if(key==Qt::Key_S) {
        if(r.bottom()<=height-s) {
            first = 3;
            t_moveS->start(t);
            if(ownTank->getVehicleID()==1) {
                t_moveA->stop();
                t_moveW->stop();
                t_moveD->stop();
            }
        }
    }
    if(key==Qt::Key_D) {
        if(r.right()<=width-s) {
            first = 4;
            t_moveD->start(t);
            if(ownTank->getVehicleID()==1) {
                t_moveA->stop();
                t_moveS->stop();
                t_moveW->stop();
            }
        }
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
    if(!ownTank->isSpawned()||ownTank->getVehicleID()==1) return;
    if(key==Qt::Key_W) {
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
