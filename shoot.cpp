#include "shoot.h"

Shoot::Shoot(Tank *t, Network *n, QPoint *aim, QObject *parent) : QObject(parent)
{
    t_cool = new QTimer();
    t_main = new QTimer();
    click = false;
    mpos = aim;
    this->t = t;
    this->network = n;
    w = new Weapon(75,500);
    connect(t_cool,SIGNAL(timeout()),this,SLOT(on_tcool()));
    connect(t_main,SIGNAL(timeout()),this,SLOT(on_tmain()));
    t_main->start(50);
}

void Shoot::on_tcool()
{
    isCool = false;
    t_cool->stop();
}

void Shoot::on_tmain()
{
    if(click&&!isCool) {
        if(!t_cool->isActive()) {
            isCool = true;
            t_cool->start(w->getCoolDown());
        }
        int v;
        if(t->getMoved()) {
            v = w->getVel()+t->getSpeed();
        } else {
            v = w->getVel();
        }
        network->send("|1#"+t->getName()+"#"+QString::number(mpos->x(),'f',0)+
                      "#"+QString::number(mpos->y(),'f',0)+"#"+
                      QString::number(v,'f',0)+"#");
    }
}

void Shoot::MousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton) {
        click = true;
    }
}

void Shoot::MouseReleaseEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton) {
        click = false;
    }
}
