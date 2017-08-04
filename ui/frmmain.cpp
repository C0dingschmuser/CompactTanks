#include "frmmain.h"
#include "ui_frmmain.h"

FrmMain::FrmMain(QWidget *parent) :
    QOpenGLWidget(parent),
    ui(new Ui::FrmMain)
{
    ui->setupUi(this);
    //setUpdateBehavior(UpdateBehavior(PartialUpdate));
    /*QSurfaceFormat f;
    f.setSwapBehavior(QSurfaceFormat::SingleBuffer);
    f.setSwapInterval(1000);
    setFormat(f);*/
    bmessage = false;
    tab = false;
    login = new FrmLogin();
    login->show();
    this->hide();
    width = 2880;
    height = 2160;
    scaleX = 1.0;
    scaleY = 1.0;
    transX = 468;
    transY = 288;
    chatActive = false;
    isConnected=true;
    fullscreen = false;
    aim = new QPoint();
    mpos = new QPoint();
    ownTank = new Tank(QRect(-200,-200,40,40),"");
    QFontDatabase d;
    d.addApplicationFont(":/font/Pixeled.ttf");
    font = d.font("Pixeled","Normal",12);
    worker = new Worker(ownTank,aim,width,height,font,static_cast<QWidget*>(this));
    t_spawn = new QTimer();
    t_death = new QTimer();
    t_draw = new QTimer();
    t_expAn = new QTimer();
    t_chat = new QTimer();
    t_killMessage = new QTimer();
    t_hit = new QTimer();
    t_time = new QTimer();
    t_draw->setTimerType(Qt::PreciseTimer);
    sound = new Sound();
    lowGraphics = false;
    workerThread = new QThread();
    ui->edtChat->setStyleSheet("QLineEdit { background: rgba(0, 255, 255, 0);}");
    connect(t_chat,SIGNAL(timeout()),this,SLOT(on_tchat()));
    connect(t_death,SIGNAL(timeout()),this,SLOT(on_tdeath()));
    connect(t_draw,SIGNAL(timeout()),this,SLOT(on_tdraw()));
    connect(t_killMessage,SIGNAL(timeout()),this,SLOT(on_tkillMessage()));
    connect(t_hit,SIGNAL(timeout()),this,SLOT(on_thit()));
    connect(t_spawn,SIGNAL(timeout()),this,SLOT(on_tspawn()));
    connect(t_time,SIGNAL(timeout()),this,SLOT(on_ttime()));
    connect(worker,SIGNAL(spawn()),this,SLOT(on_spawn()));
    connect(worker,SIGNAL(newBullet(Bullet*)),this,SLOT(on_newBullet(Bullet*)));
    connect(worker,SIGNAL(newPlayer(Tank*)),this,SLOT(on_newPlayer(Tank*))); //bei neuem spieler aufrufen
    connect(worker,SIGNAL(delPlayer(int)),this,SLOT(on_delPlayer(int)));
    connect(worker,SIGNAL(newlvlObj(Terrain*)),this,SLOT(on_newlvlObj(Terrain*)));
    connect(worker,SIGNAL(delBullet(int)),this,SLOT(on_delBullet(int)));
    //connect(network,SIGNAL(syncBullet(int,int,int,int)),this,SLOT(on_syncBullet(int,int,int,int)));
    connect(worker,SIGNAL(delObjs()),this,SLOT(on_delObjs()));
    connect(worker,SIGNAL(disconnected()),this,SLOT(on_disconnect()));
    connect(worker,SIGNAL(killMessage(QString)),this,SLOT(on_killMessage(QString)));
    connect(worker,SIGNAL(kick()),this,SLOT(on_kick()));
    connect(worker,SIGNAL(visible(bool)),this,SLOT(on_visible(bool)));
    //connect(network,SIGNAL(capobj(int,int,int)),this,SLOT(on_capobj(int,int,int)));
    connect(worker,SIGNAL(fullscreen()),this,SLOT(on_fullscreen()));
    connect(worker,SIGNAL(tab()),this,SLOT(on_tab()));
    connect(worker,SIGNAL(connFail()),this,SLOT(on_connFail()));
    connect(worker,SIGNAL(newMap(QVector<Terrain*>)),this,SLOT(on_newMap(QVector<Terrain*>)));
    connect(worker,SIGNAL(connSuccess()),this,SLOT(on_connSuccess()));
    connect(worker,SIGNAL(wrongData()),this,SLOT(on_wrongData()));
    connect(worker,SIGNAL(shot()),this,SLOT(on_shot()));
    connect(worker,SIGNAL(hit(Tank*,int)),this,SLOT(on_hit(Tank*,int)));
    connect(worker,SIGNAL(death()),this,SLOT(on_death()));
    connect(worker,SIGNAL(msgbox(QString,QString)),this,SLOT(on_msgBox(QString,QString)));
    connect(worker,SIGNAL(otherDeath(QRect)),this,SLOT(on_otherDeath(QRect)));
    connect(worker,SIGNAL(chatS(QString)),this,SLOT(on_chat(QString)));
    connect(t_expAn,SIGNAL(timeout()),this,SLOT(on_tExpAn()));
    connect(login,SIGNAL(connectWithData(QString,QString,double)),this,SLOT(on_connectData(QString,QString,double)));
    msgCount = 0;
    for(int i=0;i<74;i++) {
        expAnPixmap.append(QPixmap(":/images/animation/explosion/"+QString::number(i)+".png"));
    }
    this->setCursor(QPixmap(":/images/tank/cursor.png"));
    tree = QPixmap(":/images/area/obj0.png");
    grass = QPixmap(":/images/area/obj9.png");
    minimap = QPixmap(":/images/gui/minimap.png");
    grid = QPixmap(":/images/area/obj2g.png");
    sSpawn = QPixmap(":/images/gui/sSpawn.png");
    sCap = QPixmap(":/images/gui/sCap.png");
    tanksMenu = QPixmap(":/images/gui/tanks.png");
    t_draw->start(10);
    t_hit->start(10);
    //t_bullet->start(5);
}

FrmMain::~FrmMain()
{
    worker->close();
    workerThread->quit();
    if(!workerThread->wait(3000)) {
        workerThread->terminate();
        workerThread->wait();
    }
    for(int i=0;i<tanks.size();i++) {
        delete tanks[i];
    }
    for(int i=0;i<lvlObjs.size();i++) {
        delete lvlObjs[i];
    }
    //disconnect(network,SIGNAL(disconnect()),this,SLOT(on_disconnect()));
    delete t_draw;
    delete ownTank;
    //delete move;
    //delete network;
    delete aim;
    delete mpos;
    delete ui;
    QApplication::exit();
}

void FrmMain::on_connectData(QString username, QString pw, double volume)
{
    sound->setVolume(volume);
    worker->connectToServer(username,pw);
}

void FrmMain::on_connSuccess()
{
    isConnected = false;
    worker->moveToThread(workerThread);
    workerThread->start();
    initializeGL();
    login->hide();
    spawns = worker->getSpawns();
    t_expAn->start(6);
    t_chat->start(15000);
    t_time->start(5000);
    this->show();
}

void FrmMain::on_wrongData()
{
    QMessageBox::information(login,"FEHLER","Falscher Benutzername und/oder Passwort");
    login->reset();
}

void FrmMain::on_connFail()
{
    //disconnect(worker,SIGNAL(disconnected()),this,SLOT(on_disconnect()));
    login->fail();
    //QApplication::exit();
}

void FrmMain::on_disconnect()
{
    QMessageBox::information(this,"FEHLER","Verbindung zum Server getrennt!");
    QApplication::exit();
}

void FrmMain::on_tExpAn()
{
    if(!expAn.size()) return;
    for(int i=0;i<expAn.size();i++) {
        int step = expAn[i]->getStep()+1;
        if(step>73) {
            step = 0;
        }
        expAn[i]->setStep(step);
    }
    for(int i=0;i<expAn.size();i++) {
        if(!expAn[i]->getStep()) {
            delete expAn[i];
            expAn.removeAt(i);
            break;
        }
    }
}

void FrmMain::on_ttime()
{
    msgCount = 0;
}

void FrmMain::on_otherDeath(QRect rect)
{
    expAn.append(new ExpAnimation(rect));
}

void FrmMain::on_shot()
{
    sound->playShot();
}

void FrmMain::on_hit(Tank *t, int dmg)
{
    Q_UNUSED(t);
    animations.append(Animation("-"+QString::number(dmg,'f',0),aim->x()-5,aim->y()-15));
}

void FrmMain::on_tspawn()
{
    sAstep++;
    if(sAstep>=200) {
        t_spawn->stop();
        ownTank->setSpawned(true);
        transX = (ownTank->getRect().x()-940)*-1;
        transY = (ownTank->getRect().y()-520)*-1;
    } else {
        //transX = (ownTank->getRect().x()-940)*-1;
        //transY = (ownTank->getRect().y()-520)*-1;
        viewRect = QRect(-469-504,-288-504,width+936+576+360,height+576+504);
        //468,288
        double diffX = (double)worker->getDifference((double)468,(double)(ownTank->getRect().x()-940)*-1)/200;
        double diffY = (double)worker->getDifference((double)288,(double)(ownTank->getRect().y()-520)*-1)/200;
        if(ownTank->getRect().x()>468) {
            transX-=diffX;
        } else {
            transX+=diffX;
        }
        if(ownTank->getRect().y()>288) {
            transY-=diffY;
        } else {
            transY+=diffY;
        }
        diffX = (double)worker->getDifference(1920,width+936)/200;
        diffY = (double)worker->getDifference(1080,height+576)/200;
        scaleX = double(this->geometry().width()/(double(width+936)-diffX*sAstep));
        scaleY = double(this->geometry().height()/(double(height+576)-diffY*sAstep));
    }
}

void FrmMain::on_tdeath()
{
    sAstep++;
    if(sAstep>=200) {
        t_death->stop();
        //ownTank->setSpawned(true);
        transX = 468;
        transY = 288;
    } else {
        //transX = (ownTank->getRect().x()-940)*-1;
        //transY = (ownTank->getRect().y()-520)*-1;
        viewRect = QRect(-468-504,-288-504,width+936+576+360,height+576+504);
        //468,288
        double diffX = (double)worker->getDifference((double)468,(double)(ownTank->getDeathPoint().x()-940)*-1)/200;
        double diffY = (double)worker->getDifference((double)288,(double)(ownTank->getDeathPoint().y()-520)*-1)/200;
        if(468>ownTank->getDeathPoint().x()) {
            transX-=diffX;
        } else {
            transX+=diffX;
        }
        if(288>ownTank->getDeathPoint().y()) {
            transY-=diffY;
        } else {
            transY+=diffY;
        }

        diffX = (double)worker->getDifference(1920,width+936)/(double)200;
        diffY = (double)worker->getDifference(1080,height+576)/(double)200;
        scaleX = double(this->geometry().width()/(double(1920)+diffX*sAstep));
        scaleY = double(this->geometry().height()/(double(1080)+diffY*sAstep));
    }
}

void FrmMain::on_spawn()
{
    sAstep = 0;
    t_spawn->start(10);
}

void FrmMain::on_death()
{
    sAstep = 0;
    //scaleX = double(this->geometry().width()/double(width+936));
    //scaleY = double(this->geometry().height()/double(height+576));
    t_death->start(10);
    //transX = 468;
    //transY = 288;
}

void FrmMain::on_thit()
{
    for(int i=0;i<animations.size();i++) {
        int x = animations[i].getCoords().x();
        int y = animations[i].getCoords().y()-1;
        if(animations[i].getCount()==30) {
            animations.removeAt(i);
        } else {
            animations[i].setCoords(x,y);
        }
    }
}

void FrmMain::on_killMessage(QString message)
{
    messages.append(message);
    if(!t_killMessage->isActive()) {
        killMessage = true;
        t_killMessage->start(1000);
    }
}

void FrmMain::on_tkillMessage()
{
    if(!messages.size()) {
        killMessage = false;
        t_killMessage->stop();
    } else {
        messages.removeAt(0);
            if(!messages.size()) {
                killMessage = false;
                t_killMessage->stop();
            }

    }
}

void FrmMain::on_chat(QString message)
{
    chat.append(message);
    if(chat.size()>24) {
        chat.removeAt(0);
    }
}

void FrmMain::on_tchat()
{
    if(chat.size()>0) chat.removeAt(0);
}

void FrmMain::on_tmessage()
{
    messages.append("aaaa");
}

void FrmMain::on_newPlayer(Tank *t)
{
    tanks.append(t);
}

void FrmMain::on_newlvlObj(Terrain *t)
{
    Terrain *te = t;
    lvlObjs.append(te);
    if(te->getType()==2) capObjs.append(lvlObjs.size()-1);
}

void FrmMain::on_newBullet(Bullet *b)
{
    bullets.append(b);
}

void FrmMain::on_delBullet(int pos)
{
    //qDebug()<<pos;
    //qDebug()<<bullets.size();
    bullets.removeAt(pos);
}

void FrmMain::on_delPlayer(int pos)
{
    tanks.removeAt(pos);
}

void FrmMain::on_delObjs()
{
    for(int i=0;i<lvlObjs.size();i++) {
        delete lvlObjs[i];
    }
    lvlObjs.resize(0);
}

void FrmMain::on_tdraw()
{
    if(!QApplication::activeWindow()) {
        worker->notActive();
    }
    ownTank->setAngle((int)qRadiansToDegrees(atan2(aim->y()-ownTank->getRect().center().y(),
                                 aim->x()-ownTank->getRect().center().x()))*-1);
    update();
}

void FrmMain::on_kick()
{
    disconnect(worker,SIGNAL(disconnected()),this,SLOT(on_disconnect()));
    QMessageBox::information(this,"FEHLER","Du wurdest gekickt!");
    exit(1);
}

void FrmMain::on_fullscreen()
{

    if(!fullscreen) {
        fullscreen = true;
        this->showFullScreen();
    } else {
        fullscreen = false;
        this->showNormal();
    }
}

void FrmMain::on_tab()
{
    if(tab) {
        tab = false;
    } else {
        tab = true;
    }
}

void FrmMain::on_visible(bool visible)
{

}

void FrmMain::on_newMap(QVector<Terrain*> lvlObjs)
{
    this->lvlObjs = lvlObjs;
}

bool FrmMain::contains(QString data,QString c)
{
    bool ok = false;
    for(int i=0;i<c.size();i++) {
        if(data.contains(c.at(i))) {
            ok = true;
        }
    }
    return ok;
}

void FrmMain::closeEvent(QCloseEvent *e)
{
    Q_UNUSED(e)
    worker->close();
}

void FrmMain::on_msgBox(QString title, QString text)
{
    QMessageBox::information(this,title,text);
}

void FrmMain::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    if(isConnected) {
        return;
    }
    /*qDebug()<<"--------";
    qDebug()<<ownTank->getRect().x();
    qDebug()<<ownTank->getRect().y();*/
    //qDebug()<<this->geometry().width();
    int startPos = 0;
    int endPos = 1200;
    if(ownTank->isSpawned()) {
        viewRect = QRect(ownTank->getRect().center().x()-960,
                               ownTank->getRect().center().y()-540,2100,1250);
        scaleX = double(this->geometry().width()/double(1920));
        scaleY = double(this->geometry().height()/double(1080));
    } else {
        if(!t_spawn->isActive()&&!t_death->isActive()) {
            viewRect = QRect(-468,-288,width+936,height+576);
            scaleX = double(this->geometry().width()/double(width+936));
            scaleY = double(this->geometry().height()/double(height+576));
        }
    }
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.scale(scaleX,scaleY);
    if(ownTank->isSpawned()) {
        transX = (ownTank->getRect().x()-940)*-1;
        transY = (ownTank->getRect().y()-520)*-1;
        painter.translate(transX,transY);
        int tPos = (viewRect.x()/72+(viewRect.y()/72*40));
        int ePos = ((viewRect.x()+2100)/72+((viewRect.y()+1250)/72*40));
        if(tPos>=0) startPos = tPos;
        if(ePos<1200) endPos = ePos;
        worker->setViewRect(viewRect,startPos,endPos);
    } else {
        worker->setViewRect(viewRect,startPos,endPos);
        painter.translate(transX,transY);
    }
    worker->setScale(scaleX,scaleY,transX,transY);
    QPoint m;
    m.setX(this->mapFromGlobal(QCursor::pos()).x()/scaleX);
    m.setY(this->mapFromGlobal(QCursor::pos()).y()/scaleY);
    mpos->setX(m.x());
    mpos->setY(m.y());
    if(ownTank->isSpawned()) {
        this->aim->setX(ownTank->getRect().x()+mpos->x()-940);
        this->aim->setY(ownTank->getRect().y()+mpos->y()-520);
    } else {
        this->aim->setX(mpos->x()-transX);
        this->aim->setY(mpos->y()-transY);
    }
    painter.setFont(font);
    for(int i=2160+576;i>viewRect.y()-72;i-=72) {
        for(int a=2880+936;a>viewRect.x()-72;a-=72) {
            if(QRect(a,i,72,72).intersects(viewRect)&&((a<0||i<0)
                    ||(a>width-72||i>height-72))) {
                if(lowGraphics) {
                    painter.setPen(Qt::NoPen);
                    painter.setBrush(QColor(119,214,126));
                    painter.drawRect(a,i,72,72);
                } else {
                    painter.drawPixmap(a,i,72,72,grass);
                }
            }
        }
    }
    painter.setPen(Qt::darkGray);
    painter.setBrush(Qt::darkGray);
    painter.drawRect(-10,-10,width+20,10);
    painter.drawRect(-10,height,width+20,10);
    painter.drawRect(-10,-10,10,height+10);
    painter.drawRect(width,-10,10,height+10);
    for(int i=2160+576;i>viewRect.y()-72;i-=72) {
        for(int a=2880+936;a>viewRect.x()-72;a-=72) {
            if(QRect(a,i,72,72).intersects(viewRect)&&((a<0||i<0)
                    ||(a>width-72||i>height-72))) {
                if(lowGraphics) {
                    painter.setPen(Qt::NoPen);
                    painter.setBrush(QColor(17,91,39));
                    painter.drawRect(a,i,72,72);
                } else {
                    painter.drawPixmap(a,i,72,72,tree);
                }
            }
        }
    }
    painter.setPen(Qt::NoPen);
    for(int i=startPos;i<endPos;i++) {
        if(lvlObjs[i]->getRect().intersects(viewRect)) {
            if(lvlObjs[i]->getType()==2) {
                int owner = lvlObjs[i]->getOwner();
                int cp = lvlObjs[i]->getAmount();
                painter.setBrush(QColor(255,255,255));
                painter.drawRect(lvlObjs[i]->getRect());
                if(cp) {
                    if(owner==ownTank->getTeam()) {
                        painter.setBrush(QColor(0,255,0));
                    } else {
                        painter.setBrush(QColor(255,0,0));
                    }
                    painter.drawEllipse(QPoint(lvlObjs[i]->getRect().center().x()+1,lvlObjs[i]->getRect().center().y()+1),cp/3,cp/3);
                }
            }
            if(lvlObjs[i]->getType()>0) {
                if(lowGraphics) {
                    painter.setPen(Qt::NoPen);
                    switch(lvlObjs[i]->getType()) {
                        case 1:
                            painter.setBrush(QColor(124,124,124));
                        break;
                        case 3:
                            painter.setBrush(QColor(119,214,126));
                        break;
                        case 4:
                            painter.setBrush(QColor(119,214,126));
                    }
                    painter.drawRect(lvlObjs[i]->getRect());
                } else {
                    painter.drawPixmap(lvlObjs[i]->getRect(),lvlObjs[i]->getPixmap());
                }
                if(lvlObjs[i]->getType()==4) {
                    painter.setBrush(QColor(255,0,0,150));
                    if(lvlObjs[i]->getRect().x()<1000&&ownTank->getTeam()==1) painter.setBrush(QColor(0,255,0,150));
                    if(lvlObjs[i]->getRect().x()>1000&&ownTank->getTeam()==2) painter.setBrush(QColor(0,255,0,150));
                    painter.drawRect(lvlObjs[i]->getRect());
                }
            } else {
                painter.drawPixmap(lvlObjs[i]->getRect(),lvlObjs[i]->getGrassPixmap());
            }
        }
    }
    if(ownTank->isSpawned()||t_spawn->isActive()) ownTank->drawTank(painter,ownTank,true);
    for(int i=0;i<tanks.size();i++) {
        if(tanks[i]->getRect().intersects(viewRect)&&tanks[i]->getRect().x()>0&&!tanks[i]->isHidden()) {
            tanks[i]->drawTank(painter,ownTank,true);
        }
    }
    for(int i=0;i<expAn.size();i++) {
        int step = expAn[i]->getStep();
        QRect rect = expAn[i]->getRect();
        painter.drawPixmap(rect,expAnPixmap[step]);
    }
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    for(int i=0;i<bullets.size();i++) {
        if(bullets[i]->getEnabled()) {
            painter.drawEllipse(bullets[i]->get().center(),3,3);
        }
    }
    for(int i=startPos;i<endPos;i++) {
        if(lvlObjs[i]->getRect().intersects(viewRect)) {
            if(!lvlObjs[i]->getType()) {
                if(lowGraphics) {
                    painter.setPen(Qt::NoPen);
                    painter.setBrush(QColor(17,91,39));
                    painter.drawRect(lvlObjs[i]->getRect());
                } else {
                    painter.drawPixmap(lvlObjs[i]->getRect(),lvlObjs[i]->getPixmap());
                }
            }
        }
    }
    //-------------DEBUG----------------
    //for(int i=0;i<lvlObjs.size();i++) {
    //    painter.drawPixmap(lvlObjs[i]->getRect(),QPixmap(":/images/area/grid.png"));
    //}
    //-------------/DEBUG----------------
    //painter.setBrush(Qt::transparent);
    //painter.drawRect(viewRect);
    /*QPainterPath path;
    QPainterPath inner;
    path.addRect(0,0,1280,720);
    inner.addEllipse(ownTank->getRect().center(),viewRange,viewRange);
    path = path.subtracted(inner);
    painter.fillPath(path,QBrush(QColor(100,100,100,200)));
    painter.setBrush(Qt::transparent);
    painter.drawEllipse(ownTank->getRect().center(),viewRange,viewRange);*/

    //painter.setBrush(QColor(255,255,0,50));
    painter.setPen(Qt::blue);
    QFont f = painter.font();
    f.setPointSize(15);
    painter.setFont(f);
    for(int i=0;i<animations.size();i++) {
        painter.drawText(QPoint(aim->x()-15,aim->y()-animations[i].getCount()-17),animations[i].getText());
    }
    painter.resetTransform();
    /*painter.setPen(Qt::red);
    painter.drawLine(0,540,1920,540);
    painter.drawLine(960,0,960,1080);*/
    painter.scale(scaleX,scaleY);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    if(ownTank->isSpawned()) {
        int health = ownTank->getHealth();
        int maxHealth = ownTank->getHealth(1);
        int offset = 400;
        painter.drawRect(18,998,offset+4,2);
        painter.drawRect(18,998+52,offset+4,2);
        painter.drawRect(18,998,2,52);
        painter.drawRect(18+offset+2,998,2,52);
        //painter.drawRect(18,998,404,54);
        if(health>maxHealth*0.8) {
            painter.setBrush(QColor(34,177,76));
        } else if(health>maxHealth*0.6) {
            painter.setBrush(QColor(181,230,29));
        } else if(health>maxHealth*0.4) {
            painter.setBrush(QColor(255,242,0));
        } else if(health>maxHealth*0.2) {
            painter.setBrush(QColor(223,89,0));
        } else if(health>0) {
            painter.setBrush(QColor(237,28,36));
        }
        painter.drawRect(20,1000,400*((double)health/maxHealth),50);
        f = painter.font();
        f.setPointSize(25);
        painter.setFont(f);
        painter.setPen(Qt::black);
        painter.drawText(190,1045,QString::number(health));
        painter.setPen(Qt::NoPen);
        //Minimap start
        painter.setOpacity(0.75);
        int x = 1920-1920*0.2;
        int y = 1080-1080*0.25;
        int w = 1920*0.2;
        int h = 1080*0.25;
        painter.drawPixmap(x,y,1920*0.2,1080*0.25,minimap);
        painter.setBrush(Qt::black);
        painter.drawRect(x-2,y-2,w+2,2);
        painter.drawRect(x-2,y-2,2,h+2);
        painter.translate(x,y);
        painter.scale(1920*0.2/width,1080*0.25/height);
        for(int i=0;i<capObjs.size();i++) {
            int num = capObjs[i];
            if(lvlObjs[num]->getOwner()==0) {
                painter.setBrush(Qt::white);
            } else if(lvlObjs[num]->getOwner()==ownTank->getTeam()) {
                painter.setBrush(QColor(0,255,0));
            } else {
                painter.setBrush(QColor(255,0,0));
            }
            painter.drawRect(lvlObjs[num]->getRect());
            if(lvlObjs[num]->getAmount()<100) {
                painter.drawPixmap(lvlObjs[num]->getRect(),grid);
            }
        }
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::blue);
        painter.drawRect(ownTank->getRect());
        for(int i=0;i<tanks.size();i++) {
            if(tanks[i]->getTeam()==ownTank->getTeam()) {
                painter.setBrush(QColor(0,255,0));
            } else {
                painter.setBrush(QColor(255,0,0));
            }
            QRect rect = tanks[i]->getRect();
            if(rect.x()!=-200&&!tanks[i]->isHidden()) {
                painter.drawRect(rect);
            }
        }
        painter.setOpacity(1);
        //minimap end
    } else {
        if(t_spawn->isActive()||t_death->isActive()) return;
        painter.translate(transX,transY);
        int selected = worker->getSelected();
        if(selected<100&&selected>-1) {
            painter.drawPixmap(spawns[selected].x()-14,spawns[selected].y()-21,172,259,sSpawn);
        } else if(selected>=100) {
            int num = selected-100;
            painter.drawPixmap(lvlObjs[num]->getRect().x()-7,lvlObjs[num]->getRect().y()-7,86,86,sCap);
        }
        painter.resetTransform();
        painter.scale(scaleX,scaleY);
        QFont f = painter.font();
        f.setPointSize(128);
        painter.setFont(f);
        painter.setPen(Qt::black);
        if(ownTank->getType()) {
            painter.drawText(900,220,"SELECT SPAWN");
        } else {
            painter.drawText(900,220,"SELECT TANK");
        }
        painter.drawPixmap(500,2490,300,216,tanksMenu);
    }


    painter.resetTransform();
    painter.scale(scaleX,scaleY);
    //painter.drawRect(0,0,1920,1110);
    if(killMessage) {
        QFont f = painter.font();
        f.setPointSize(24);
        painter.setFont(f);
        painter.setPen(Qt::white);
        painter.setBrush(Qt::white);
        for(int i=1;i<messages.size();i++) {
            painter.drawText(3300, i*50,messages[i-1]);
        }
    }
    if(chat.size()) {
        QFont f = painter.font();
        int size,x,y,space;
        if(ownTank->isSpawned()) {
            size = 12;
            x = 20;
            y = 750;
            space = 25;
        } else {
            size = 32;
            x = 30;
            y = 1890;
            space = 75;
        }
        f.setPointSize(size);
        painter.setFont(f);
        for(int i=chat.size();i>0;i--) {
            QString text = chat[chat.size()-i];
            if(text.contains(ownTank->getName()+": ")) {
                painter.setPen(Qt::black);
            } else {
                painter.setPen(Qt::darkBlue);
            }
            painter.drawText(x, y-(i*space),text);
        }
    }
    if(tab) {
        int offset = 350;
        painter.setPen(Qt::red);
        painter.drawText(0+offset,48,"Name");
        painter.drawText(402+offset,48,"Kills");
        painter.drawText(462+offset,48,"Tode");
        painter.drawLine(QPoint(0+offset,50),QPoint(520+offset,50));
        painter.drawLine(QPoint(400+offset,30),QPoint(400+offset,210));
        painter.drawLine(QPoint(460+offset,30),QPoint(460+offset,210));
        painter.drawText(0+offset,64,ownTank->getName());
        painter.drawText(402+offset,64,QString::number(ownTank->getKills(),'f',0));
        painter.drawText(462+offset,64,QString::number(ownTank->getDeaths(),'f',0));
        painter.drawText(462+offset,128,QString::number(ownTank->getCoins(),'f',0));
    }
}

void FrmMain::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    //QCursor::setPos(mapToGlobal(QPoint(1280/2,720/2)));
}

void FrmMain::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Enter||e->key()==Qt::Key_Return) {
        if(chatActive) {
            ui->edtChat->setStyleSheet("QLineEdit { background: rgba(0, 255, 255, 0);}");
            this->setFocus();
            if(msgCount<3) {
                worker->chat(ui->edtChat->text());
                msgCount++;
            } else {
                on_chat("Kein Spam!");
            }
            ui->edtChat->setText("");
            ui->edtChat->setEnabled(false);
            chatActive = false;
        } else {
            QRect rect = ui->edtChat->geometry();
            if(rect.x()!=this->window()->width()*0.0078||rect.y()!=this->window()->height()*0.68)
                ui->edtChat->setGeometry(this->window()->width()*0.0078,this->window()->height()*0.68,rect.width(),rect.height());
            ui->edtChat->setStyleSheet("QLineEdit { background: rgba(0, 255, 255, 255);}");
            ui->edtChat->setEnabled(true);
            ui->edtChat->setFocus();
            chatActive = true;
        }
        return;
    }
    worker->keyP(e);
}

void FrmMain::keyReleaseEvent(QKeyEvent *e)
{
    worker->keyR(e);
}

void FrmMain::mousePressEvent(QMouseEvent *e)
{
    bool ok = false;
    if(t_death->isActive()||t_spawn->isActive()) ok = true;
    worker->mPrs(e,ok);
}

void FrmMain::mouseReleaseEvent(QMouseEvent *e)
{
    worker->mRls(e);
}
