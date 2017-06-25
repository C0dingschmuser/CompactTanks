#include "movement.h"

Movement::Movement(Tank *t, int width, int height, QObject *parent) : QObject(parent)
{
    this->width = width;
    this->height = height;
    ownTank = t;
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

void Movement::on_tmove()
{
    if(!t_moveW->isActive()&&!t_moveA->isActive()&&
            !t_moveS->isActive()&&!t_moveD->isActive()) {
        ownTank->setMoved(false);
    }
}

void Movement::on_tmoveW() {
    QRect r = ownTank->getRect();
    int s = ownTank->getSpeed();
    if(!t_moveA->isActive()&&!t_moveD->isActive()) {
        if(r.y()>=0+s) {
            bool ok=false;
            for(int i=0;i<lvlObjs.size();i++) {
                if(QRect(r.x(),r.y()-s,40,40).intersects(lvlObjs[i]->getRect())) {
                    if(!lvlObjs[i]->getType()) {
                        ok = true;
                        break;
                    }
                }
            }
            if(!ok) {
                ownTank->w();
            }
        }
        ownTank->setMoved(true);
    }
}

void Movement::on_tmoveA() {
    QRect r = ownTank->getRect();
    int s = ownTank->getSpeed();
    if(!t_moveW->isActive()&&!t_moveS->isActive()) {
        if(r.x()>=0+s) {
            bool ok=false;
            for(int i=0;i<lvlObjs.size();i++) {
                if(QRect(r.x()-s,r.y(),40,40).intersects(lvlObjs[i]->getRect())) {
                    if(!lvlObjs[i]->getType()) {
                        ok = true;
                        break;
                    }
                }
            }
            if(!ok) {
                ownTank->a();
            }
        }
        ownTank->setMoved(true);
    }
}

void Movement::on_tmoveS() {
    QRect r = ownTank->getRect();
    int s = ownTank->getSpeed();
    if(!t_moveA->isActive()&&!t_moveD->isActive()) {
        if(r.bottom()<=height-s*2) {
            bool ok=false;
            for(int i=0;i<lvlObjs.size();i++) {
                if(QRect(r.x(),r.y()+s,40,40).intersects(lvlObjs[i]->getRect())) {
                    if(!lvlObjs[i]->getType()) {
                        ok = true;
                        break;
                    }
                }
            }
            if(!ok) {
                ownTank->s();
            }
        }
        ownTank->setMoved(true);
    }
}

void Movement::on_tmoveD() {
    QRect r = ownTank->getRect();
    int s = ownTank->getSpeed();
    if(!t_moveW->isActive()&&!t_moveS->isActive()) {
        if(r.right()<=width-s*2) {
            bool ok=false;
            for(int i=0;i<lvlObjs.size();i++) {
                if(QRect(r.x()+s,r.y(),40,40).intersects(lvlObjs[i]->getRect())) {
                    if(!lvlObjs[i]->getType()) {
                        ok = true;
                        break;
                    }
                }
            }
            if(!ok) {
                ownTank->d();
            }
        }
        ownTank->setMoved(true);
    }
}

void Movement::keyPressEvent(QKeyEvent *e, QVector<Terrain *> lvlObjs)
{
    this->lvlObjs = lvlObjs;
    QRect r = ownTank->getRect();
    int s = ownTank->getSpeed();
    if(e->key()==Qt::Key_W) {
        if(r.y()>=0+s) {
            t_moveW->start(8);
        }
    }
    if(e->key()==Qt::Key_A) {
        if(r.x()>=0+s) {
            t_moveA->start(8);
        }
    }
    if(e->key()==Qt::Key_S) {
        if(r.bottom()<=height-s) {
            t_moveS->start(8);
        }
    }
    if(e->key()==Qt::Key_D) {
        if(r.right()<=width-s) {
            t_moveD->start(8);
        }
    }
    if(e->key()==Qt::Key_Tab) {
        emit tab();
    }
}

void Movement::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_W) {
        t_moveW->stop();
        ownTank->setMoved(false);
    }
    if(e->key()==Qt::Key_A) {
        t_moveA->stop();
        ownTank->setMoved(false);
    }
    if(e->key()==Qt::Key_S) {
        t_moveS->stop();
        ownTank->setMoved(false);
    }
    if(e->key()==Qt::Key_D) {
        t_moveD->stop();
        ownTank->setMoved(false);
    }
    if(e->key()==Qt::Key_Tab) {
        emit tab();
    }
    if(e->key()==Qt::Key_F11) {
        emit fullscreen();
    }
}

void Movement::stop()
{
    t_moveW->stop();
    t_moveA->stop();
    t_moveS->stop();
    t_moveD->stop();
}
