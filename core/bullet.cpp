#include "bullet.h"

Bullet::Bullet()
{

}

Bullet::~Bullet()
{

}

Bullet::Bullet(int x, int y, double x2, double y2, int vel, QString shooter, QColor color, int onImpact)
{
    this->color = color;
    this->coll = true;
    this->x = x;
    this->y = y;
    this->elapsed = 0;
    double distance = qSqrt(qPow(qFabs(x2-(double)x),2)+qPow(qFabs(y2-(double)y),2));
    this->x2 = (x2-x)/distance;
    this->y2 = (y2-y)/distance;
    //this->x += x2*5;
    //this->y += y2*5;
    this->vel = vel/2;
    this->shooter = shooter;
    this->onImpact = onImpact;
    this->dmg = 20;
}

void Bullet::setEnabled(bool enabled)
{
    this->enabled = enabled;
}

void Bullet::setColl(bool coll)
{
    this->coll = coll;
}

void Bullet::update()
{
    elapsed += 1;
    this->x += x2*vel*0.04;
    this->y += y2*vel*0.04;
}

void Bullet::sync(int x, int y,int elapsed)
{
    if(elapsed!=-1) {
        this->elapsed = elapsed;
    }
    if(getDistance(QPoint(this->x,this->y),QPoint(x,y))>20) {
        this->x = x;
        this->y = y;
    }
}

void Bullet::setDmg(int dmg)
{
    this->dmg = dmg;
}

double Bullet::getDistance(QPoint p1, QPoint p2)
{
    return (double)sqrt(pow(p1.x()-p2.x(),2)+pow(p1.y()-p2.y(),2));
}

bool Bullet::getEnabled()
{
    return this->enabled;
}

bool Bullet::getColl()
{
    return this->coll;
}

QString Bullet::getShooter()
{
    return this->shooter;
}

QRect Bullet::get()
{
    return(QRect(x,y,2,2));
}

QColor Bullet::getColor()
{
    return this->color;
}

int Bullet::getImpactID()
{
    return this->onImpact;
}

int Bullet::getElapsed()
{
    return this->elapsed;
}

int Bullet::getVel()
{
    return this->vel*0.1;
}

int Bullet::getDmg()
{
    return this->dmg;
}
