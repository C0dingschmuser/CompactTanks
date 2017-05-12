#include "shoot.h"

Shoot::Shoot(Tank *t,Network *n, QObject *parent) : QObject(parent)
{
    t_cool = new QTimer();
    t_main = new QTimer();
    click = false;
    this->t = t;
    this->network = n;
    w = new Weapon(30,200);
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
        network->send("|1#"+t->getName()+"#"+QString::number(mpos->x(),'f',0)+
                      "#"+QString::number(mpos->y(),'f',0)+"#"+
                      QString::number(w->getVel(),'f',0)+"#");
    }
}

void Shoot::MousePressEvent(QMouseEvent *e,QPoint *mpos)
{
    if(e->button()==Qt::LeftButton) {
        click = true;
        this->mpos = mpos;
    }
}

void Shoot::MouseReleaseEvent(QMouseEvent *e,QPoint *mpos)
{
    if(e->button()==Qt::LeftButton) {
        click = false;
        this->mpos = mpos;
    }
}
