#include "tank.h"

QPoint Tank::getCenter()
{
    return QPoint(body->GetPosition().x,body->GetPosition().y);
}

Tank::Tank(QRect rect, QString name, b2World *world, int team, bool locked,QObject *parent) :
    QObject(parent)
{
    this->rect = rect;
    this->name = name;
    speed = 1;
    dir = 1;
    this->currentID = 0;
    stationary = true;
    angle = 0;
    kills = 0;
    deaths = 0;
    timer = 10;
    moved = true;
    visible = true;
    spawned = false;
    viewRange = 120;
    this->locked = locked;
    health = 100;
    hidden = false;
    spotted = 1;
    turnAngle = 0;
    shootmode = 0;
    coins = 0;
    type = 0;
    endSpeed = 0;
    b2BodyDef BodyDef;
    BodyDef.type = b2_dynamicBody; //this will be a dynamic body
    BodyDef.position.Set(300, 200); //set the starting position
    BodyDef.angle = 0; //set the starting angle
    body = world->CreateBody(&BodyDef);
    grid = QPixmap("images/area/grid2.png");
    this->team = team;
    img = QPixmap("images/tank/"+QString::number(type,'f',0)+"/tank1.png");
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

bool Tank::isStationary()
{
    return stationary;
}

bool Tank::getHeal()
{
    return heal;
}

QPolygonF Tank::getPolygon(int type)
{
    QRectF r = rect;
    int deg = turnAngle;
    QTransform transform;
    switch(type) {
        case 1: // move w
            r.moveTo(r.x()+vx,r.y()+vy);
        break;
        case 2: //move s
            r.moveTo(r.x()-vx,r.y()-vy);
        break;
        case 3: //turn right
            deg++;
        break;
        case 4: //turn left
            deg--;
        break;
    }
    QPointF center = r.center();
    r.translate(-center);
    QPolygonF pol(r);
    for(int i=0;i<pol.size();i++) {
        QPointF p = pol.at(i);
        double tempX = p.x()*qCos(qDegreesToRadians((double)deg)) - p.y()*qSin(qDegreesToRadians((double)deg));
        double tempY = p.x()*qSin(qDegreesToRadians((double)deg)) + p.y()*qCos(qDegreesToRadians((double)deg));
        pol.replace(i,QPointF(tempX+center.x(),tempY+center.y()));
    }
    return pol;
}

QRectF Tank::getRect()
{
    if(body!=NULL&&body!=nullptr) {
        return QRectF(body->GetPosition().x-(rect.width()/2),body->GetPosition().y-(rect.height()/2),rect.width(),rect.height());
    } else {
        return QRectF(-200,-200,rect.width(),rect.height());
    }
}

b2Body *Tank::getBody()
{
    return body;
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
    if(!a) {
        this->dir = 1;
    }
    this->rect.moveTo(rect.x(),rect.y()-speed);
}

void Tank::a(bool a)
{
    if(!a) {
        this->dir = 2;
    }
    this->rect.moveTo(rect.x()-speed,rect.y());
}

void Tank::s(bool a)
{
    if(!a) {
        this->dir = 3;
    }
    this->rect.moveTo(rect.x(),rect.y()+speed);
}

void Tank::d(bool a)
{
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
    img = QPixmap("images/tank/"+QString::number(type,'f',0)+"/tank1.png");
    if(vehicleID==1&&type!=1) return;
    turret = QPixmap("images/tank/"+QString::number(type,'f',0)+"/turm.png");
}

void Tank::setData(int type, int speed, int health, int bvel, int reload, int width, int height, int barrelLength, bool heal, int camo, int viewrange, int vehicleID)
{
    body->DestroyFixture(body->GetFixtureList());
    b2PolygonShape boxShape;
    boxShape.SetAsBox(width/2,height/2);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = 20;
    if(vehicleID!=1) {
        body->SetLinearDamping(3);
    } else {
        fixtureDef.filter.maskBits = 0x0000;
    }
    body->CreateFixture(&fixtureDef);
    body->SetAngularDamping(15);
    this->vehicleID = vehicleID;
    setType(type);
    this->speed = (((double)speed*3.6)*5); //kmh zu m/s -> 1m = 5px -> m/s*5 = px/s
    this->health = health;
    this->maxHealth = health;
    this->bvel = bvel;
    this->reload = reload;
    this->width = width;
    this->height = height;
    this->barrelLength = barrelLength;
    this->heal = heal;
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

int Tank::getVehicleID()
{
    return vehicleID;
}

int Tank::getTurnAngle()
{
    int w = qRadiansToDegrees(body->GetAngle());
    int an = w/360;
    an = w-(an*360);
    return an;
}

QPoint Tank::getDeathPoint()
{
    return deathPoint;
}

QPoint Tank::getShootPoint()
{
    QPoint p = getCenter();
    return p;
}

void Tank::setUsername(QString text)
{
    this->name = text;
}

void Tank::setShootmode(int shootmode)
{
    this->shootmode = shootmode;
}

void Tank::setTurnAngle(int angle)
{
    if(angle>360) {
        angle = 0;
    } else if(angle<0) {
        angle = 360;
    }
    this->turnAngle = angle;
    body->SetTransform(body->GetPosition(),qDegreesToRadians((double)angle));
}

void Tank::setDir(int dir)
{
    this->dir = dir;
}

int Tank::getShootmode()
{
    return shootmode;
}

void Tank::drawTank(QPainter &p, Tank *own, bool barrel)
{
    if(name==NULL||(!spawned&&this!=own)) return;
    QColor rcolor;
    QRect r;
    int deg = 0;
    int xt = (body->GetPosition().x-rect.width()/2);
    int yt = (body->GetPosition().y-rect.height()/2);
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
    p.setBrush(rcolor);
    p.setPen(Qt::NoPen);
    //p.drawRect(r);
    p.save();
    p.translate(xt+rect.width()/2,yt+rect.height()/2);
    //p.rotate(qRadiansToDegrees(body->GetAngle())+90);
    p.rotate(getTurnAngle()+90);
    p.drawPixmap(-rect.width()/2,-rect.height()/2,rect.width(),rect.height(),img);
    p.restore();
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
            case 9:
                p.translate(xt+25,yt+25);
                p.rotate(-angle);
                p.drawPixmap(-25,-30,100,60,turret);
            break;
        }
        p.restore();
        /*p.setPen(Qt::red);
        p.setBrush(Qt::red);
        p.drawRect(xt+25,yt+30,1,1);
        /*p.setPen(Qt::green);
        p.setBrush(Qt::green);
        p.drawRect(xt+30,yt+25,1,1);*/
    }
    p.setOpacity(1.0);
}

void Tank::move(int type)
{
    if(!locked) {
        vx = qCos(body->GetAngle())*speed/1.75;
        vy = qSin(body->GetAngle())*speed/1.75;
    }
    if(!type||locked) {
        body->SetLinearVelocity(b2Vec2(vx,vy));
        //body->ApplyForce(b2Vec2(vx,vy),body->GetWorldCenter(),true);
    } else {
        body->SetLinearVelocity(b2Vec2(-vx,-vy));
    }
}

QString Tank::getName()
{
    return this->name;
}

QString Tank::toString()
{
    rect = getRect();
    turnAngle = getTurnAngle();
    b2Vec2 vel = body->GetLinearVelocity();
    return this->name + "#" + QString::number(rect.x(),'f',3)+"#"+QString::number(rect.y(),'f',3)+"#"+
            QString::number(turnAngle,'f',0)+"#"+QString::number(angle,'f',0)+"#"+QString::number(dir,'f',0)+"#"+
            QString::number(vel.x,'f',3)+"#"+QString::number(vel.y,'f',3)+"#";
}

QPixmap Tank::getIMG()
{
    return this->img;
}

void Tank::teleport(int x, int y)
{
    this->dir = 1;
    this->targetPos = QPoint(x,y);
    rect.moveTo(x,y);
    body->SetTransform(b2Vec2(x+rect.width()/2,y+rect.height()/2),qDegreesToRadians((double)angle));
}

void Tank::setAll(double x, double y, int turnAngle, int health, int diff, double vx, double vy)
{
    this->health = health;
    dir = diff;
    rect = getRect();
    if((rect.x()==-200&&rect.y()==-200)||(x==-200&&y==-200)) {
        this->turnAngle=0;
        rect.moveTo(x,y);
        body->SetTransform(b2Vec2(x-rect.width()/2,y-rect.height()/2),qDegreesToRadians((double)turnAngle));
        targetPos = QPoint(x,y);
    } else {
        if(rect.x()!=x||rect.y()!=y) {
            rect.moveTo(x,y);
            body->SetTransform(b2Vec2(x+rect.width()/2,y+rect.height()/2),qDegreesToRadians((double)turnAngle));
        }
        this->turnAngle = turnAngle;

    }
    this->vx = vx;
    this->vy = vy;
    //this->rect.moveTo(x,y);
}

void Tank::setAll(double x, double y)
{
    rect.moveTo(x,y);
    body->SetTransform(b2Vec2(x+rect.width()/2,y+rect.height()/2),qDegreesToRadians((double)angle));
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

void Tank::setAngle(double angle)
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

void Tank::setVehicleID(int id)
{
    this->vehicleID = id;
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

double Tank::getAngle()
{
    return angle;
}

double Tank::getSpeed()
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

double Tank::getVx()
{
    return vx;
}

double Tank::getVy()
{
    return vy;
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
