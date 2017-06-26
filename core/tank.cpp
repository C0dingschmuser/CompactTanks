#include "tank.h"

Tank::Tank()
{

}

Tank::Tank(QRect rect, QString name, int team)
{
    this->rect = rect;
    this->name = name;
    speed = 2;
    dir = 1;
    angle = 0;
    kills = 0;
    deaths = 0;
    moved = true;
    visible = true;
    viewRange = 120;
    health = 100;
    spotted = 1;
    coins = 0;
    grid = QPixmap(":/images/area/grid2.png");
    this->team = team;
    for(int i=0;i<4;i++) {
        QPixmap p = QPixmap(":/images/tank/tank"+QString::number(i+1,'f',0)+".png");
        imgs.append(p);
    }
    currentImg = imgs[0];
}

Tank::~Tank()
{

}

bool Tank::getVisible()
{
    return visible;
}

bool Tank::getMoved()
{
    return this->moved;
}

QRect Tank::getRect()
{
    return this->rect;
}

void Tank::setVisible(bool visible)
{
    this->visible = visible;
}

void Tank::w(bool a)
{
    //currentImg = imgs[0];
    if(!a) {
        this->dir = 1;
    }
    this->rect.moveTo(rect.x(),rect.y()-speed);
}

void Tank::a(bool a)
{
    //currentImg = imgs[1];
    if(!a) {
        this->dir = 2;
    }
    this->rect.moveTo(rect.x()-speed,rect.y());
}

void Tank::s(bool a)
{
    //currentImg = imgs[2];
    if(!a) {
        this->dir = 3;
    }
    this->rect.moveTo(rect.x(),rect.y()+speed);
}

void Tank::d(bool a)
{
    //currentImg = imgs[3];
    if(!a) {
        this->dir = 4;
    }
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

void Tank::setSpotted(int spotted)
{
    this->spotted = spotted;
}

int Tank::getSpotted()
{
    return this->spotted;
}

void Tank::drawTank(QPainter &p, Tank *own, bool barrel)
{
    QColor rcolor;
    QRect r;
    int xt = rect.x();
    int yt = rect.y();
    if(!visible) {
        p.setOpacity(0.5);
    }
    switch(color) {
        case 0: //rot
            rcolor = QColor(255,0,0);
        break;
        case 1: //grün
            rcolor = QColor(0,255,0);
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
            r = QRect(xt+5,yt+4,30,34);
        break;
        case 2:
            r = QRect(xt+4,yt+4,34,30);
        break;
        case 3:
            r = QRect(xt+5,yt+2,30,34);
        break;
        case 4:
            r = QRect(xt+2,yt+6,34,29);
        break;
    }
    p.setBrush(rcolor);
    p.setPen(Qt::NoPen);
    p.drawRect(r);
    p.drawPixmap(xt,yt,40,40,imgs[dir-1]);
    QFont f = p.font();
    f.setPointSize(6);
    p.setFont(f);
    QFontMetrics m(f);
    QRect br = m.boundingRect(name);
    p.setPen(Qt::NoPen);
    p.drawRect(xt,yt-br.height()*0.7-2,br.width(),br.height()*0.7);
    p.setPen(Qt::NoPen);
    if(health>80) {
        p.setBrush(QColor(34,177,76));
    } else if(health>60) {
        p.setBrush(QColor(181,230,29));
    } else if(health>40) {
        p.setBrush(QColor(255,242,0));
    } else if(health>20) {
        p.setBrush(QColor(223,89,0));
    } else if(health>0) {
        p.setBrush(QColor(237,28,36));
    }
    p.drawRect(xt+1,yt+rect.height()+3,40*((double)health/100),10);
    p.setPen(Qt::black);
    p.drawText(QPoint(xt,yt-br.height()*0.7+11),name);
    p.drawText(xt+8,yt+rect.height()+13,QString::number(health,'f',0));
    if(barrel) {
        QPen pen;
        pen.setColor(Qt::black);
        pen.setWidth(5);
        p.setPen(pen);
        p.drawLine(getBarrel(xt,yt));
        p.setPen(Qt::black);
    }
    p.setOpacity(1.0);
}

void Tank::move()
{
    //rect.moveTo(targetPos.x(),targetPos.y());
    int speed = this->speed/2;
    if(rect.x()<targetPos.x()-speed-1) {
        rect.moveTo(rect.x()+speed,rect.y());
        //targetPos.setX(targetPos.x()+1);
    } else if(rect.x()>targetPos.x()+speed-1) {
        rect.moveTo(rect.x()-speed,rect.y());
        //targetPos.setX(targetPos.x()-1);
    }
    if(rect.y()<targetPos.y()-speed-1) {
        rect.moveTo(rect.x(),rect.y()+speed);
        //targetPos.setY(targetPos.y()+1);
    } else if(rect.y()>targetPos.y()+speed-1) {
        rect.moveTo(rect.x(),rect.y()-speed);
        //targetPos.setY(targetPos.y()-1);
    }
}

QString Tank::getName()
{
    return this->name;
}

QString Tank::toString()
{
    return this->name + "#" + QString::number(rect.x(),'f',0)+"#"+QString::number(rect.y(),'f',0)+"#"+
            QString::number(dir,'f',0)+"#"+QString::number(angle,'f',0)+"#";
}

QPixmap Tank::getIMG()
{
    return this->currentImg;
}

void Tank::teleport(int x, int y)
{
    this->dir = 1;
    this->targetPos = QPoint(x,y);
    rect.moveTo(x,y);
}

void Tank::setAll(int x, int y, int dir, int health, int diff)
{
    this->health = health;
    if(!dir) {
        dir = 1;
    }
    if((rect.x()==-200&&rect.y()==-200)||(x==-200&&y==-200)) {
        //qDebug()<<"a";
        this->dir = 1;
        rect.moveTo(x,y);
        targetPos = QPoint(x,y);
    } else {
        int s = (speed*diff);
        switch(dir) {
            case 1:
                if(getDifference(y,y-s)>s) {
                    y-=s;
                }
            break;
            case 2:
                if(getDifference(x,x-s)>s) {
                    x-=s;
                }
            break;
            case 3:
                if(getDifference(y,y+s)>s) {
                    y+=s;
                }
            break;
            case 4:
                if(getDifference(x,x+s)>s) {
                    x+=s;
                }
            break;
        }
        targetPos = QPoint(x,y);
        this->dir = dir;
    }
    //this->rect.moveTo(x,y);
}

void Tank::setAll(int x, int y)
{
    rect.moveTo(x,y);
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
    this->angle = angle;
    /*if(this->angle<0) {
        this->angle=360;
    } else if(this->angle>360) {
        this->angle=0;
    }*/
}

void Tank::setCoins(int coins)
{
    this->coins = coins;
}

void Tank::setName(QString name)
{
    this->name = name;
}

int Tank::getDifference(int v1, int v2)
{
    int diff = 0;
    if(v1>v2) {
        diff = v1-v2;
    } else if(v2>v1) {
        diff = v2-v1;
    }
    return diff;
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

int Tank::getHealth()
{
    return this->health;
}

int Tank::getTeam()
{
    return this->team;
}

int Tank::getCoins()
{
    return this->coins;
}

void Tank::setHealth(int health)
{
    this->health = health;
}

void Tank::setTeam(int team)
{
    this->team = team;
}

QLineF Tank::getBarrel(int xt, int yt)
{
    QLineF barrel;
    barrel.setP1(QPoint(xt+20,yt+20));
    barrel.setP2(QPoint(xt+20,yt+5));
    barrel.setAngle(angle);
    return barrel;
}
