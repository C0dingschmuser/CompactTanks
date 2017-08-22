#include "worker.h"

Worker::Worker(Tank *ownTank, QPoint *aim, int width, int height, QFont f, QWidget *mainWindow, QObject *parent) : QObject(parent)
{
    this->width = width;
    this->height = height;
    this->ownTank = ownTank;
    this->aim = aim;
    selected = -1;
    classID = 0;
    startPos = 0;
    endPos = 1200;
    respawn = 0;
    this->mainWindow = mainWindow;
    t_bullet = new QTimer(this);
    t_main = new QTimer(this);
    t_conn = new QTimer(this);
    t_id = new QTimer(this);
    t_select = new QTimer(this);
    t_visible = new QTimer(this);
    t_move = new QTimer(this);
    t_respawn = new QTimer(this);
    t_main->setTimerType(Qt::PreciseTimer);
    move = new Movement(this->ownTank,this->width,this->height);
    //37.120.177.121
    network = new Network(this->ownTank,tanks,QHostAddress("37.120.177.121")); //ändern
    shoot = new Shoot(this->ownTank,network,this->aim);
    tankWindow = new FrmTanks(f,ownTank);
    connect(t_bullet,SIGNAL(timeout()),this,SLOT(on_tbullet()));
    connect(network,SIGNAL(playerDeath()),this,SLOT(on_playerDeath()));
    connect(network,SIGNAL(spawn()),this,SIGNAL(spawn()));
    connect(network,SIGNAL(newPlayer(Tank*)),this,SLOT(on_newPlayer(Tank*)));
    connect(network,SIGNAL(delPlayer(int)),this,SLOT(on_delPlayer(int)));
    connect(network,SIGNAL(delObjs()),this,SLOT(on_delObjs()));
    connect(network,SIGNAL(newBullet(Bullet*)),this,SLOT(on_newBullet(Bullet*)));
    connect(network,SIGNAL(delBullet(int)),this,SLOT(on_delBullet(int)));
    connect(network,SIGNAL(syncBullet(int,int)),this,SLOT(on_syncBullet(int,int)));
    connect(network,SIGNAL(visible(int)),this,SLOT(on_visible(int)));
    connect(network,SIGNAL(message(QString,int)),this,SIGNAL(message(QString,int)));
    connect(network,SIGNAL(killMessage(QString)),this,SIGNAL(killMessage(QString)));
    connect(network,SIGNAL(kick()),this,SIGNAL(kick()));
    connect(network,SIGNAL(capobj(int,int,int)),this,SLOT(on_capobj(int,int,int)));
    connect(network,SIGNAL(setT(int)),this,SLOT(on_setT(int)));
    connect(network,SIGNAL(pos(Tank*,int,int,int,int,int,int,int)),this,SLOT(on_pos(Tank*,int,int,int,int,int,int,int)));
    connect(network,SIGNAL(conn(int)),this,SLOT(on_conn(int)));
    connect(network,SIGNAL(hit(Tank*,QString)),this,SIGNAL(hit(Tank*,QString)));
    connect(network,SIGNAL(stats(int,int,int,int,int,int,int,int,double,double,double,int,int,int,int)),this,SLOT(on_db(int,int,int,int,int,int,int,int,double,double,double,int,int,int,int)));
    connect(network,SIGNAL(spawn(Tank*)),this,SLOT(on_spawn(Tank*)));
    connect(network,SIGNAL(otherDeath(QRect)),this,SIGNAL(otherDeath(QRect)));
    connect(network,SIGNAL(chat(QString)),this,SIGNAL(chatS(QString)));
    connect(network,SIGNAL(ping(int)),this,SIGNAL(ping(int)));
    connect(network,SIGNAL(teamCP(int,int)),this,SIGNAL(teamCP(int,int)));
    connect(network,SIGNAL(reset(int)),this,SLOT(on_reset(int)));
    connect(network,SIGNAL(ownHit()),this,SIGNAL(ownHit()));
    connect(network,SIGNAL(changelog(int)),this,SLOT(on_changelog(int)));
    connect(move,SIGNAL(fullscreen()),this,SIGNAL(fullscreen()));
    connect(move,SIGNAL(tab()),this,SIGNAL(tab()));
    connect(shoot,SIGNAL(newBullet(Bullet*)),this,SLOT(on_newBullet(Bullet*)));
    connect(t_main,SIGNAL(timeout()),this,SLOT(on_tmain()));
    connect(t_conn,SIGNAL(timeout()),this,SLOT(on_tconn()));
    connect(t_id,SIGNAL(timeout()),this,SLOT(on_tid()));
    connect(t_select,SIGNAL(timeout()),this,SLOT(on_tselect()));
    connect(t_visible,SIGNAL(timeout()),this,SLOT(on_tvisible()));
    connect(t_move,SIGNAL(timeout()),this,SLOT(on_tmove()));
    connect(t_respawn,SIGNAL(timeout()),this,SLOT(on_tRespawn()));
    t_bullet->start(2);
    t_main->start(4);
    t_move->start(5);
    //loadMap();
}

Worker::~Worker()
{
    disconnect(network,SIGNAL(disconnect()),this,SIGNAL(disconnected()));
    delete move;
    delete network;
    delete tankWindow;
}

void Worker::on_disconnect()
{
    emit disconnected();
}

void Worker::connectToServer(QString username, QString password, QString version)
{
    //t_conn->start(200);
    if(network->connectToServer(username,password,version)) {
        this->username = username;

        t_conn->stop();
    } else {
        emit connFail();
    }
}

void Worker::on_conn(int id)
{
    if(id==1) {
        connect(network,SIGNAL(disconnect()),this,SLOT(on_disconnect()));
        ownTank->setName(username);
        loadMap();
        t_id->start(100);
        t_visible->start(100);
        emit connSuccess();
    } else {
        emit wrongData(id);
    }
}

void Worker::on_changelog(int size)
{
    int lsize = QFileInfo("changelog.txt").size();
    if(lsize!=size) emit changeStart();
}

void Worker::on_tid()
{
    for(int i=startPos;i<endPos;i++) {
        if(ownTank->getRect().intersects(lvlObjs.at(i)->getRect())) {
            ownTank->setID(lvlObjs[i]->getType());
            break;
        }
    }
}

void Worker::on_pos(Tank *p, int x, int y, int dir, int health, int angle, int spotted, int stimer)
{
    if(!p->isSpawned()) return;
    int diff = getDifference(stimer,timer);
    /*qDebug()<<stimer;
    qDebug()<<timer;
    qDebug()<<diff/p->getTimer();
    qDebug()<<"-------------";*/
    p->setAll(x,y,dir,health,diff/p->getTimer());
    p->setAngle(angle);
    p->setSpotted(spotted);
    /*qDebug()<<"--------";
    qDebug()<<timer;
    qDebug()<<stimer;*/
}

void Worker::on_db(int id, int dmg, int reload, int speed, int health, int width, int height, int barrelLength, double softTerrRes, double hardTerrRes, double treeTerrRes, int treeColl, int vel, int camo, int viewrange)
{
    dbTanks.append(new dbTank("",dmg,reload,speed,health,width,height,barrelLength,softTerrRes,hardTerrRes,treeTerrRes,treeColl,vel,camo,viewrange));
    tankWindow->setDB(dbTanks);
}

void Worker::on_tselect()
{
    if(ownTank->isSpawned()) return;
    selected = -1;
    int max,min;
    if(ownTank->getTeam()==1) {
        min = 0;
        max = 2;
    } else {
        min = 2;
        max = 4;
    }
    for(int i=min;i<max;i++) {
        if(spawns[i].intersects(QRect(aim->x(),aim->y(),1,1))) {
            selected = i;
            break;
        }
    }
    if(selected==-1) {
        for(int i=0;i<capObjs.size();i++) {
            if(lvlObjs[capObjs[i]]->getRect().intersects(QRect(aim->x(),aim->y(),1,1))) {
                if(lvlObjs[capObjs[i]]->getOwner()==ownTank->getTeam()&&
                        lvlObjs[capObjs[i]]->getAmount()==100) {
                    selected = 100+capObjs[i];
                    break;
                }
            }
        }
    }
}

int Worker::getSelected()
{
    return selected;
}

void Worker::on_tconn()
{
    t_conn->stop();
    //emit connFail();
}

void Worker::on_newPlayer(Tank *t)
{
    tanks.append(t);
    emit newPlayer(t);
}

void Worker::on_delPlayer(int pos)
{
    tanks.removeAt(pos);
    emit delPlayer(pos);
}

void Worker::on_delObjs()
{
    for(int i=0;i<lvlObjs.size();i++) {
        delete lvlObjs[i];
    }
    lvlObjs.resize(0);
    emit delObjs();
}

void Worker::on_newBullet(Bullet *b)
{
    bullets.append(b);
    int type = 0;
    Tank *tmp = network->sucheTank(b->getShooter());
    if(tmp) {
        type = tmp->getType();
    } else {
        type = ownTank->getType();
    }
    if(b->get().intersects(viewRect)) emit shot(type);
    emit newBullet(b);
}

void Worker::on_delBullet(int pos)
{
    if(bullets.size()-1>=pos) {
        bullets.removeAt(pos);
        emit delBullet(pos);
    }
}

void Worker::on_syncBullet(int pos, int dmg)
{
    //keine weiterleitung da pointer
    if(bullets.size()-1>=pos) {
        bullets[pos-1]->setDmg(dmg);
    }
}

void Worker::on_tbullet()
{
    if(bullets.size()>0) {
        for(int i=0;i<bullets.size();i++) {
            if(bullets[i]->getEnabled()) {
                bullets[i]->update();
            }
            for(int a=0;a<tanks.size();a++) {
                if(bullets[i]->getShooter()!=tanks[a]->getName()) {
                    if(bullets[i]->get().intersects(tanks[a]->getRect())) {
                        bullets[i]->setEnabled(false);
                    }
                }
            }
            int x = bullets[i]->get().center().x();
            int y = bullets[i]->get().center().y();
            int pos = (x/72+(y/72*40));
            if(x<0||y<0||x>width||y>height||!lvlObjs[pos]->getType()) bullets[i]->setEnabled(false);
        }
    }
}

void Worker::on_visible(int v)
{
    bool ok=false;
    for(int i=startPos;i<endPos;i++) {
        if(ownTank->getRect().intersects(lvlObjs.at(i)->getRect())) {
            if(lvlObjs[i]->getType()>0&&lvlObjs[i]->getType()<3) {
                ok = true;
                break;
            }
        }
    }
    if(v) {
        ownTank->setVisible(true);
        ok = true;
    } else {
        ok = false;
        ownTank->setVisible(false);
    }
    //emit visible(ok);
}

void Worker::on_tvisible()
{
    for(int i=0;i<tanks.size();i++) {
        bool ok=false;
        for(int a=0;a<lvlObjs.size();a++) {
            if(tanks[i]->getRect().intersects(lvlObjs.at(a)->getRect())&&
                    lvlObjs[a]->getType()>0&&lvlObjs[a]->getType()<3) {
                ok = true;
                break;
            }
        }
        if(tanks[i]->getTeam()==ownTank->getTeam()) {
            if(tanks[i]->getSpotted()) {
                tanks[i]->setVisible(true);
            } else {
                tanks[i]->setVisible(false);
            }
        } /*else {
            if(ok) {
                tanks[i]->setHidden(false);
            } else {
                tanks[i]->setHidden(true);
            }
        }*/
    }
}

void Worker::on_capobj(int num, int owner, int cp)
{
    if(num>lvlObjs.size()-1) return;
    lvlObjs[num]->setOwner(owner);
    lvlObjs[num]->setAmount(cp);
}

void Worker::on_setT(int timer)
{
    this->timer = timer;
}

void Worker::on_tmain()
{
    timer+= 4;
    network->setTimer(timer);
    if(timer>5000) {
        timer = 0;
        network->setTimer(0);
    }
    for(int i=0;i<tanks.size();i++) {
        if(tanks[i]->getRect().x()>0) {
            //tanks[i]->move();
        }
    }
}

void Worker::on_playerDeath()
{
    t_respawn->start(1000);
    emit death();
}

void Worker::doSpawn()
{
    //ownTank->setSpawned(true);
    ownTank->setDamage(dbTanks[ownTank->getType()-1]->getDmg());
    network->send("|0#"+QString::number(selected,'f',0)+"#"+QString::number(ownTank->getType(),'f',0)+"#~");
}

void Worker::on_spawn(Tank *t)
{
    int id = t->getType();
    t->setData(id,dbTanks[id-1]->getSpeed(),dbTanks[id-1]->getHealth(),dbTanks[id-1]->getVel(),dbTanks[id-1]->getReload(),
            dbTanks[id-1]->getWidth(),dbTanks[id-1]->getHeight(),dbTanks[id-1]->getBarrelLength(),dbTanks[id-1]->getTreeColl(),
            dbTanks[id-1]->getCamo(),dbTanks[id-1]->getViewrange());
}

void Worker::on_tmove()
{
    for(int i=0;i<tanks.size();i++) {
        if(tanks[i]->isSpawned()&&tanks[i]->getRect().x()!=-200) {
            tanks[i]->move();
        }
    }
}

void Worker::on_reset(int team)
{
    emit resetMatch(team);
    for(int i=0;i<capObjs.size();i++) {
        int num = capObjs[i];
        lvlObjs[num]->setAmount(0);
        lvlObjs[num]->setOwner(0);
    }
    for(int i=0;i<tanks.size();i++) {
        tanks[i]->setSpawned(false);
        tanks[i]->setAll(-200,-200);
    }
    t_respawn->start(1000);
}

void Worker::on_tRespawn()
{
    if(respawn>0) {
        respawn--;
        if(!respawn) t_respawn->stop();
    }
}

void Worker::chat(QString message)
{
    if(message=="") return;
    if(contains(message,"|*~")) {
        emit chatS("Fehler: Zeichen verboten!");
        return;
    }
    if(message.size()>40) {
        emit chatS("Fehler: Text zu lang!");
        return;
    }
    message.replace("ß","&0");
    message.replace("ä","&1");
    message.replace("ö", "&2");
    message.replace("ü","&3");
    message.replace("Ä","&11");
    message.replace("Ö", "&22");
    message.replace("Ü","&33");
    network->send("|3#"+ownTank->getName()+": "+message);
}

void Worker::close()
{
    tankWindow->hide();
}

void Worker::moveObjects(QThread *thread)
{
    move->moveToThread(thread);
    network->moveToThread(thread);
    shoot->moveToThread(thread);
}

void Worker::loadMap()
{
    QFile file(":/map/map.dat");
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString line;
    while(!in.atEnd()) {
        line += in.readLine();
    }
    QStringList basic = line.split(",");
    int max2 = width/72;
    int max1 = height/72;
    for(int i=0;i<max1;i++) {
        for(int a=0;a<max2;a++) {
            Terrain *obj = new Terrain(0+(72*a),0+(72*i),72,72,getType(basic.at(lvlObjs.size()).toInt()),getType(basic.at(lvlObjs.size()+(max1*max2)).toInt()));
            lvlObjs.append(obj);
            if(obj->getType()==2) capObjs.append(lvlObjs.size()-1);
        }
    }
    for(int i=0;i<lvlObjs.size();i++) {
        emit newlvlObj(lvlObjs[i]);
    }
    spawns.append(QRect(0,144,144,216)); //links oben
    spawns.append(QRect(0,1800,144,216)); //links unten
    spawns.append(QRect(2736,504,144,216)); //rechts oben
    spawns.append(QRect(2736,1296,144,216)); //rechts unten
    move->setSpawns(spawns);
    classRects.append(QRect(500,2490,300,216));
    classRects.append(QRect(900,2490,300,216));
    t_select->start(50);
    //emit newMap(lvlObjs);
}

QVector<QRect> Worker::getSpawns()
{
    return spawns;
}

int Worker::getType(int type) {
    int t = type-1;
    return t;
}

bool Worker::contains(QString data,QString c)
{
    bool ok = false;
    for(int i=0;i<c.size();i++) {
        if(data.contains(c.at(i))) {
            ok = true;
        }
    }
    return ok;
}

int Worker::getDifference(int v1, int v2)
{
    int diff = 0;
    if(v1>v2) {
        diff = v1-v2;
    } else if(v2>v1) {
        diff = v2-v1;
    }
    return diff;
}

int Worker::getClassID()
{
    return classID;
}

double Worker::getDifference(double v1,double v2)
{
    double diff = 0;
    if(v1>v2) {
        diff = v1-v2;
    } else if(v2>v1) {
        diff = v2-v1;
    }
    return diff;
}

void Worker::keyP(QKeyEvent *e)
{
    //if(!ownTank->isSpawned()) return;
    move->keyPressEvent(e,lvlObjs,startPos,endPos);
}

void Worker::keyR(QKeyEvent *e)
{
    //if(!ownTank->isSpawned()) return;
    move->keyReleaseEvent(e);
}

void Worker::mPrs(QMouseEvent *e, bool SD)
{
    if(ownTank->isSpawned()) {
        shoot->MousePressEvent(e);
    } else {
        if(e->button()==Qt::LeftButton) {
            if(SD) return;
            if(selected>-1) {
                if(!ownTank->getType()) {
                    emit msgbox("FEHLER","Wähle zuerst einen Panzer aus!");
                } else {
                    if(respawn) return;
                    respawn = 3;
                    if(tankWindow->isVisible()) tankWindow->hide();
                    doSpawn();
                }
            }
            QRect coll = classRects[0];
            coll.moveTo(coll.x()-transX,coll.y()-transY);
            if(coll.intersects(QRect(aim->x(),aim->y(),1,1))) {
                QPoint pos = mainWindow->pos();
                tankWindow->move(pos.x()+200,pos.y()+200);
                tankWindow->show();
                tankWindow->setFocus();
            }
        }
    }
}

void Worker::mRls(QMouseEvent *e)
{
    if(ownTank->isSpawned()) {
        shoot->MouseReleaseEvent(e);
    }
}

void Worker::setScale(double scaleX, double scaleY, int transX, int transY)
{
    this->scaleX = scaleX;
    this->scaleY = scaleY;
    this->transX = transX;
    this->transY = transY;
}

void Worker::notActive()
{
    move->stop();
}

void Worker::setViewRect(QRect viewRect, int startPos, int endPos)
{
    this->viewRect = viewRect;
    this->startPos = startPos;
    this->endPos = endPos;
}
