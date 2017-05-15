#include "tank.h"

Tank::Tank()
{

}

Tank::Tank(QRect rect, QString name)
{
    this->rect = rect;
    this->name = name;
    speed = 3;
    dir = 1;
    angle = 0;
    kills = 0;
    deaths = 0;
    moved = true;
    viewRange = 120;
    for(int i=0;i<4;i++) {
        QPixmap p = QPixmap(":/images/tank/tank"+QString::number(i+1,'f',0)+".png");
        imgs.append(p);
    }
    currentImg = imgs[0];
}

Tank::~Tank()
{

}

bool Tank::getMoved()
{
    return this->moved;
}

QRect Tank::getRect()
{
    return this->rect;
}

void Tank::w()
{
    //currentImg = imgs[0];
    angle = 90;
    this->dir = 1;
    this->rect.moveTo(rect.x(),rect.y()-speed);
}

void Tank::a()
{
    //currentImg = imgs[1];
    angle = 180;
    this->dir = 2;
    this->rect.moveTo(rect.x()-speed,rect.y());
}

void Tank::s()
{
    //currentImg = imgs[2];
    angle = 270;
    this->dir = 3;
    this->rect.moveTo(rect.x(),rect.y()+speed);
}

void Tank::d()
{
    //currentImg = imgs[3];
    angle = 360;
    this->dir = 4;
    this->rect.moveTo(rect.x()+speed,rect.y());
}

void Tank::kill()
{
    kills++;
}

void Tank::death()
{
    deaths++;
}

void Tank::setDK(int kills, int deaths)
{
    this->kills = kills;
    this->deaths = deaths;
}

void Tank::drawTank(QPainter &p)
{
    QColor rcolor;
    QRect r;
    switch(color) {
        case 0:
            rcolor = QColor(160,0,94);
        break;
        case 1:
            rcolor = QColor(238,205,125);
        break;
        case 2:
            rcolor = QColor(92,163,99);
        break;
        case 3:
            rcolor = QColor(157,139,126);
        break;
        case 4:
            rcolor = QColor(31,240,127);
        break;
        case 5:
            rcolor = QColor(200,92,112);
        break;
        case 6:
            rcolor = QColor(132,240,109);
        break;
        case 7:
            rcolor = QColor(50,192,122);
        break;
        case 8:
            rcolor = QColor(14,99,128);
        break;
        case 9:
            rcolor = QColor(255,165,0);
        break;
    }
    switch(dir) {
        case 1:
            r = QRect(rect.x()+3,rect.y()+2,14,17);
        break;
        case 2:
            r = QRect(rect.x()+2,rect.y()+3,18,14);
        break;
        case 3:
            r = QRect(rect.x()+3,rect.y()+1,14,17);
        break;
        case 4:
            r = QRect(rect.x()+1,rect.y()+3,17,14);
        break;
    }
    p.setBrush(rcolor);
    p.setPen(rcolor);
    p.drawRect(r);
    p.drawPixmap(rect,imgs[dir-1]);
}

QString Tank::getName()
{
    return this->name;
}

QString Tank::toString()
{
    return this->name + "#" + QString::number(rect.x(),'f',0)+"#"+QString::number(rect.y(),'f',0)+"#"+
            QString::number(dir,'f',0)+"#";
}

QPixmap Tank::getIMG()
{
    return this->currentImg;
}

void Tank::setAll(int x, int y, int dir)
{
    switch(dir) {
        case 1:
            rect.setWidth(40);
            rect.setHeight(50);
        break;
        case 2:
            rect.setWidth(50);
            rect.setHeight(40);
        break;
        case 3:
            rect.setWidth(40);
            rect.setHeight(50);
        break;
        case 4:
            rect.setWidth(50);
            rect.setHeight(40);
    }
    this->dir = dir;
    this->rect.moveTo(x,y);
}

void Tank::setMoved(bool m)
{
    this->moved = m;
}

void Tank::setSpeed(int speed)
{
    this->speed = speed;
}

void Tank::setColor(int color)
{
    this->color = color;
}

void Tank::setAngle(int angle)
{
    this->angle += angle;
    if(this->angle<0) {
        this->angle=360;
    } else if(this->angle>360) {
        this->angle=0;
    }
}

int Tank::getAngle()
{
    return angle;
}

int Tank::getSpeed()
{
    return this->speed;
}

int Tank::getKills()
{
    return this->kills;
}

int Tank::getDeaths()
{
    return this->deaths;
}

int Tank::getDir()
{
    return this->dir;
}

int Tank::getViewRange()
{
    return this->viewRange;
}

void Tank::setViewRange(int vr)
{
    this->viewRange = vr;
}

QLineF Tank::getBarrel()
{
    QLineF barrel;
    barrel.setP1(QPoint(rect.center().x(),rect.center().y()));
    barrel.setP2(QPoint(rect.center().x(),rect.center().y()-15));
    barrel.setAngle(angle);
    return barrel;
}
