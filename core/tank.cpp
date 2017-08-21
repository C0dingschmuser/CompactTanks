#include "tank.h"

Tank::Tank(QRect rect, QString name, int team, QObject *parent) :
    QObject(parent)
{
    this->rect = rect;
    this->name = name;
    speed = 1;
    dir = 1;
    this->currentID = 0;
    angle = 0;
    kills = 0;
    deaths = 0;
    timer = 10;
    moved = true;
    visible = true;
    spawned = false;
    viewRange = 120;
    health = 100;
    hidden = false;
    spotted = 1;
    coins = 0;
    type = 0;
    endSpeed = 0;
    grid = QPixmap(":/images/area/grid2.png");
    this->team = team;
    for(int i=0;i<4;i++) {
        QPixmap p = QPixmap(":/images/tank/"+QString::number(type,'f',0)+"/tank"+QString::number(i+1,'f',0)+".png");
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

bool Tank::isSpawned()
{
    return spawned;
}

bool Tank::isHidden()
{
    return hidden;
}

QRect Tank::getRect()
{
    return this->rect;
}

void Tank::setVisible(bool visible)
{
    this->visible = visible;
}

void Tank::setSpawned(bool spawned)
{
    this->spawned = spawned;
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

void Tank::setDeathPoint(QPoint dPoint)
{
    deathPoint = dPoint;
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

void Tank::setID(int id)
{
    this->currentID = id;
}

void Tank::setType(int type)
{
    if(this->type==type) return;
    this->type = type;
    imgs.resize(0);
    for(int i=0;i<4;i++) {
        QPixmap p = QPixmap(":/images/tank/"+QString::number(type,'f',0)+"/tank"+QString::number(i+1,'f',0)+".png");
        imgs.append(p);
    }
    currentImg = imgs[0];
    turret = QPixmap(":/images/tank/"+QString::number(type,'f',0)+"/turm.png");
}

void Tank::setData(int type, int speed, int health, int bvel, int reload, int width, int height, int barrelLength, int treeColl, int camo, int viewrange)
{
    setType(type);
    this->timer = ((double)speed/10)/2;
    this->health = health;
    this->maxHealth = health;
    this->bvel = bvel;
    this->reload = reload;
    this->width = width;
    this->height = height;
    this->barrelLength = barrelLength;
    this->treeColl = treeColl;
    this->camo = camo;
    this->viewRange = viewrange;
    rect = QRect(rect.x(),rect.y(),width,height);
}

void Tank::setDamage(int dmg)
{
    this->damage = dmg;
}

void Tank::setHidden(bool hidden)
{
    this->hidden = hidden;
}

int Tank::getDamage()
{
    return damage;
}

int Tank::getTimer()
{
    return timer;
}

int Tank::getType()
{
    return type;
}

int Tank::getReload()
{
    return this->reload;
}

int Tank::getBvel()
{
    return this->bvel;
}

int Tank::getWidth()
{
    return width;
}

int Tank::getHeight()
{
    return height;
}

int Tank::getSpotted()
{
    return this->spotted;
}

int Tank::getID()
{
    return currentID;
}

int Tank::getViewrange()
{
    return viewRange;
}

QPoint Tank::getDeathPoint()
{
    return deathPoint;
}

QPoint Tank::getShootPoint()
{
    QPoint p;
    switch(type) {
        case 1:
            p = QPoint(rect.center().x(),rect.center().y()+2);
        break;
        case 2:
            p = QPoint(rect.center().x(),rect.center().y()+2);
        break;
        case 3:
            p = QPoint(rect.center().x(),rect.center().y()+2);
        break;
        case 4:
            p = QPoint(rect.center().x(),rect.center().y()+2);
        break;
        default:
            p = QPoint(rect.center().x(),rect.center().y()+2);
        break;
    }
    return p;
}

void Tank::setUsername(QString text)
{
    this->name = text;
}

void Tank::drawTank(QPainter &p, Tank *own, bool barrel)
{
    if(name==NULL||(!spawned&&this!=own)) return;
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
    //p.drawRect(r);
    p.drawPixmap(xt,yt,rect.width(),rect.height(),imgs[dir-1]);
    QFont f = p.font();
    f.setPointSize(12);
    p.setFont(f);
    QFontMetrics m(f);
    QRect br = m.boundingRect(name);
    p.setPen(Qt::NoPen);
    p.drawRect(xt-2,yt-br.height()-2,br.width()+3,(br.height()+4)/2);
    p.setPen(Qt::NoPen);
    if(health>maxHealth*0.8) {
        p.setBrush(QColor(34,177,76));
    } else if(health>maxHealth*0.6) {
        p.setBrush(QColor(181,230,29));
    } else if(health>maxHealth*0.4) {
        p.setBrush(QColor(255,242,0));
    } else if(health>maxHealth*0.2) {
        p.setBrush(QColor(223,89,0));
    } else if(health>0) {
        p.setBrush(QColor(237,28,36));
    }
    p.drawRect(xt+1,yt+rect.height()+3,rect.width()*((double)health/maxHealth),10);
    p.setPen(Qt::black);
    p.drawText(QPointF(xt,yt-br.height()/2),name);
    p.drawText(QPointF(xt+8,yt+rect.height()+13),QString::number(health,'f',0));
    if(barrel) {
        p.save();
        switch(type) {
            case 1:
                p.translate(xt+20,yt+20);
                p.rotate(-angle);
                p.drawPixmap(-20,-25,50,50,turret);
            break;
            case 2:
                p.translate(xt+25,yt+23);
                p.rotate(-angle);
                p.drawPixmap(-25,-30,100,60,turret);
            break;
            case 3:
                p.translate(xt+20,yt+20);
                p.rotate(-angle);
                p.drawPixmap(-25,-25,50,50,turret);
            break;
            case 4:
                p.translate(xt+30,yt+30);
                p.rotate(-angle);
                p.drawPixmap(-30,-25,90,50,turret);
            break;
            case 5:
                p.translate(xt+25,yt+25);
                p.rotate(-angle);
                p.drawPixmap(-35,-21,80,40,turret);
            break;
            case 6:
                p.translate(xt+20,yt+20);
                p.rotate(-angle);
                p.drawPixmap(-40,-19,80,40,turret);
            break;
            case 7:
                p.translate(xt+25,yt+25);
                p.rotate(-angle);
                p.drawPixmap(-20,-21,80,40,turret);
            break;
        }
        p.restore();
        /*p.setPen(Qt::red);
        p.setBrush(Qt::red);
        p.drawRect(xt+30,yt+30,1,1);
        /*p.setPen(Qt::green);
        p.setBrush(Qt::green);
        p.drawRect(xt+30,yt+25,1,1);*/
    }
    p.setOpacity(1.0);
}

void Tank::move()
{
    //rect.moveTo(targetPos.x(),targetPos.y());
    int speed = this->endSpeed/5;
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
        if(rect.x()!=x||rect.y()!=y) rect.moveTo(x,y);
        int tx = x;
        int ty = y;
        int s = (speed*diff);
        switch(dir) {
            case 1:
                if(getDifference(y,y-s)>s) {
                    y-=s;
                    ty-=s*2;
                }
            break;
            case 2:
                if(getDifference(x,x-s)>s) {
                    x-=s;
                    tx-=s*2;
                }
            break;
            case 3:
                if(getDifference(y,y+s)>s) {
                    y+=s;
                    ty+=s*2;
                }
            break;
            case 4:
                if(getDifference(x,x+s)>s) {
                    x+=s;
                    tx-=s*2;
                }
            break;
        }
        endSpeed = s*2;
        targetPos = QPoint(tx,ty);
        //rect.moveTo(x,y);
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
    this->speed = speed/10;
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

int Tank::getHealth(int type)
{
    if(!type) {
        return this->health;
    } else {
        return this->maxHealth;
    }
}

int Tank::getTeam()
{
    return this->team;
}

int Tank::getCoins()
{
    return this->coins;
}

void Tank::setHealth(int health, int maxHealth)
{
    this->health = health;
    if(maxHealth) {
        this->maxHealth = maxHealth;
    }
}

void Tank::setTeam(int team)
{
    this->team = team;
}

QLineF Tank::getBarrel(int xt, int yt)
{
    QLineF barrel;
    switch(type) {
        case 1:
            barrel.setP1(QPoint(xt+20,yt+20));
            barrel.setP2(QPoint(xt+20,yt+5));
        break;
        case 2:
            switch(dir) {
                case 1:
                    barrel.setP1(QPoint(xt+20,yt+27));
                    barrel.setP2(QPoint(xt+20,yt+2));
                break;
                case 2:
                    barrel.setP1(QPoint(xt+27,yt+20));
                    barrel.setP2(QPoint(xt+27,yt-5));
                break;
                case 3:
                    barrel.setP1(QPoint(xt+20,yt+13));
                    barrel.setP2(QPoint(xt+20,yt-12));
                break;
                case 4:
                    barrel.setP1(QPoint(xt+13,yt+20));
                    barrel.setP2(QPoint(xt+13,yt-5));
                break;
            }
        break;
        case 3:
            barrel.setP1(QPoint(xt+15,yt+15));
            barrel.setP2(QPoint(xt+15,yt+5));
        break;
        case 4:
            barrel.setP1(QPoint(xt+25,yt+25));
            barrel.setP2(QPoint(xt+25,yt+5));
        break;
    }
    barrel.setAngle(angle);
    return barrel;
}
