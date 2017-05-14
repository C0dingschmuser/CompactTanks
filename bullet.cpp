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
    double distance = qSqrt(qPow(qFabs(x2-x),2)+qPow(qFabs(y2-y),2));
    this->x2 = (x2-x)/distance;
    this->y2 = (y2-y)/distance;
    this->vel = vel;
    this->shooter = shooter;
    this->onImpact = onImpact;
}

void Bullet::setColl(bool coll)
{
    this->coll = coll;
}

void Bullet::update()
{
    elapsed += 1;
    this->x += x2*vel*0.1;
    this->y += y2*vel*0.1;
}

void Bullet::sync(int x, int y,int elapsed)
{
    this->elapsed = elapsed;
    this->x = x;
    this->y = y;
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
