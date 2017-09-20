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
    settings = new FrmSettings();
    this->hide();
    width = 2880;
    height = 2160;
    scaleX = 1.0;
    scaleY = 1.0;
    transX = 468;
    transY = 288;
    ping = 0;
    team1CP = 0;
    team2CP = 0;
    graphics = 0;
    maxDmg = 0;
    currentReload = 0;
    maxReload = 0;
    chatActive = false;
    isConnected = true;
    fullscreen = false;
    aim = new QPoint();
    mpos = new QPoint();
    b2Vec2 gravity(0,0);
    world = new b2World(gravity);
    ownTank = new Tank(QRect(-200,-200,40,40),"",world,-1,false);
    QFontDatabase d;
    d.addApplicationFont(":/font/Pixeled.ttf");
    font = QFont("Arial",12);//d.font("Arial","Normal",12);
    worker = new Worker(ownTank,aim,width,height,font,world,static_cast<QWidget*>(this));
    t_spawn = new QTimer();
    t_death = new QTimer();
    t_draw = new QTimer();
    t_expAn = new QTimer();
    t_chat = new QTimer();
    t_killMessage = new QTimer();
    t_hit = new QTimer();
    t_time = new QTimer();
    t_mouse = new QTimer();
    t_draw->setTimerType(Qt::PreciseTimer);
    sound = new Sound();
    lowGraphics = false;
    version = "v0.0.6.4";
    reloadRect = QRect(-200,-200,60,60);
    changeDl = new FileDownloader(QUrl("http://37.120.177.121/compacttanks/changelog.txt"));
    updateDL = new FileDownloader(QUrl("http://37.120.177.121/compacttanks/update.zip"));
    connect(updateDL,SIGNAL(downloaded()),this,SLOT(on_download()));
    connect(changeDl,SIGNAL(downloaded()),this,SLOT(on_changeDL()));
    workerThread = new QThread();

    ui->edtChat->setStyleSheet("QLineEdit { background: rgba(0, 255, 255, 0);}");
    connect(t_chat,SIGNAL(timeout()),this,SLOT(on_tchat()));
    connect(t_death,SIGNAL(timeout()),this,SLOT(on_tdeath()));
    connect(t_draw,SIGNAL(timeout()),this,SLOT(on_tdraw()));
    connect(t_killMessage,SIGNAL(timeout()),this,SLOT(on_tkillMessage()));
    connect(t_hit,SIGNAL(timeout()),this,SLOT(on_thit()));
    connect(t_spawn,SIGNAL(timeout()),this,SLOT(on_tspawn()));
    connect(t_time,SIGNAL(timeout()),this,SLOT(on_ttime()));
    connect(t_mouse,SIGNAL(timeout()),this,SLOT(on_tmouse()));
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
    connect(worker,SIGNAL(wrongData(int)),this,SLOT(on_wrongData(int)));
    connect(worker,SIGNAL(shot(int)),this,SLOT(on_shot(int)));
    connect(worker,SIGNAL(hit(Tank*,QString)),this,SLOT(on_hit(Tank*,QString)));
    connect(worker,SIGNAL(death()),this,SLOT(on_death()));
    connect(worker,SIGNAL(msgbox(QString,QString)),this,SLOT(on_msgBox(QString,QString)));
    connect(worker,SIGNAL(otherDeath(QRectF,bool)),this,SLOT(on_otherDeath(QRectF,bool)));
    connect(worker,SIGNAL(chatS(QString)),this,SLOT(on_chat(QString)));
    connect(worker,SIGNAL(ping(int)),this,SLOT(on_ping(int)));
    connect(worker,SIGNAL(teamCP(int,int)),this,SLOT(on_teamCP(int,int)));
    connect(worker,SIGNAL(resetMatch(int)),this,SLOT(on_resetMatch(int)));
    connect(worker,SIGNAL(ownHit()),this,SLOT(on_ownHit()));
    connect(worker,SIGNAL(changeStart()),this,SLOT(on_changeDLStart()));
    connect(worker,SIGNAL(reloadData(int,int)),this,SLOT(on_reload(int,int)));
    connect(worker,SIGNAL(powerup(Powerup*)),this,SLOT(on_powerup(Powerup*)));
    connect(worker,SIGNAL(delPowerup(int)),this,SLOT(on_delPowerup(int)));
    connect(worker,SIGNAL(registration(int)),this,SLOT(on_registration(int)));
    connect(t_expAn,SIGNAL(timeout()),this,SLOT(on_tExpAn()));
    connect(login,SIGNAL(connectWithData(QString,QString,double,int,bool)),this,SLOT(on_connectData(QString,QString,double,int,bool)));
    connect(login,SIGNAL(confirm(QString)),this,SLOT(on_sendData(QString)));
    connect(login,SIGNAL(sendEmail(QString)),this,SLOT(on_sendData(QString)));
    connect(settings,SIGNAL(save(double,int,bool)),this,SLOT(on_settingsSave(double,int,bool)));
    msgCount = 0;
    map = QPixmap("images/area/map.png");
    for(int i=0;i<74;i++) {
        expAnPixmap.append(QPixmap("images/animation/explosion/"+QString::number(i)+".png"));
    }
    //maxplayerid
    for(int i=0;i<9;i++) {
        classIcons.append(QPixmap("images/tank/tank"+QString::number(i+1)+".png"));
    }
    /*tanks.append(new Tank(QRect(0,0,1,1),"num1",1));
    tanks.append(new Tank(QRect(0,0,1,1),"num2",1));
    tanks.append(new Tank(QRect(0,0,1,1),"num3",2));
    tanks.append(new Tank(QRect(0,0,1,1),"num4",2));
    tanks.append(new Tank(QRect(0,0,1,1),"num5",1));*/
    this->setCursor(QPixmap("images/tank/cursor.png"));
    tree = QPixmap("images/area/obj0.png");
    //grass = QPixmap("images/area/obj9.png");
    minimap = QPixmap("images/gui/minimap.png");
    grid = QPixmap("images/area/obj2g.png");
    sSpawn = QPixmap("images/gui/sSpawn.png");
    sCap = QPixmap("images/gui/sCap.png");
    tanksMenu = QPixmap("images/gui/tanks.png");
    win = QPixmap("images/gui/win.png");
    mode0 = QPixmap("images/gui/mode0.png");
    mode1 = QPixmap("images/gui/mode1.png");
    mode0g = QPixmap("images/gui/mode0g.png");
    mode1g = QPixmap("images/gui/mode1g.png");
    t_draw->start(25);
    t_hit->start(10);
    t_mouse->start(1);
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
    delete settings;
    delete world;
    QApplication::exit();
}

void FrmMain::on_connectData(QString username, QString pw, double volume, int volume2, bool VSync)
{
    if(username!="#0") {
        ownTank->setUsername(username);
        if(!VSync) {
            QSurfaceFormat f;
            f.setSwapInterval(0);
            QSurfaceFormat::setDefaultFormat(f);
        }
        sound->setVolume(volume);
        sound->setMusicVolume(volume2);
        settings->setData(volume,volume2,VSync);
        worker->connectToServer(username,pw,version);
    } else {
        worker->connectToServer("#0");
    }
}

void FrmMain::on_connSuccess()
{
    t_draw->setInterval(5);
    sound->playMusic();
    login->setLogin(false);
    isConnected = false;
    worker->moveToThread(workerThread);
    workerThread->start();
    workerThread->setPriority(QThread::NormalPriority);
    //worker->moveObjects(workerThread);
    //this->showMaximized();
    worker->run(workerThread);
    initializeGL();
    login->hide();
    spawns = worker->getSpawns();
    t_expAn->start(5);
    t_expAn->moveToThread(workerThread);
    t_mouse->moveToThread(workerThread);
    t_time->start(5000);
    t_time->moveToThread(workerThread);
    t_chat->start(30000);
    this->show();
}

void FrmMain::on_sendData(QString email)
{
    worker->send(email);
}

void FrmMain::on_wrongData(int id)
{
    QString msg;
    switch(id) {
        case -4:
            msg = "Du wurdest gebannt!";
        break;
        case -3:
            msg = "Client outdated! Update wird jetzt heruntergeladen!";
            updateDL->start();
        break;
        case -2:
            msg = "Server voll!";
        break;
        case -1:
            msg = "Benutzer bereits angemeldet!";
        break;
        case 0:
            msg = "Falscher Benutzername und/oder Passwort";
        break;
    }

    QMessageBox::information(login,"FEHLER",msg);
    login->reset();
}

void FrmMain::on_registration(int code)
{
    login->reg(code);
}

void FrmMain::on_settingsSave(double vol1, int vol2, bool vsync)
{
    sound->setVolume(vol1);
    sound->setMusicVolume(vol2);
    QFile file;
    QString path = qApp->applicationDirPath();
    path.append("/settings.ini");
    file.setFileName(path);
    file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
    QTextStream out(&file);
    QString sel = "0";
    if(vsync) sel = "1";
    out << ownTank->getName() << "#" << sel << "#" <<
           QString::number(vol1,'f',2) << "#" << QString::number(vol2) << "#" << endl;
    file.close();
}

void FrmMain::on_download()
{
    QFile file("update.zip");
    file.open(QIODevice::WriteOnly);
    file.write(updateDL->downloadedData());
    file.close();
    QMessageBox::information(this,"Download beendet","Das Update befindet sich im ordner update.zip");
}

void FrmMain::on_changeDL()
{
    QFile file("changelog.txt");
    file.open(QIODevice::WriteOnly);
    file.write(changeDl->downloadedData());
    file.close();
    QMessageBox::StandardButton box = QMessageBox::question(this,"Changelog","Changelog anzeigen?");
    if(box==QMessageBox::Yes) {
        QDesktopServices::openUrl(QUrl::fromLocalFile("changelog.txt"));
    }
}

void FrmMain::on_changeDLStart()
{
    changeDl->start();
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

void FrmMain::on_teamCP(int team1cp, int team2cp)
{
    this->team1CP = team1cp;
    this->team2CP = team2cp;
}

void FrmMain::on_ping(int ping)
{
    this->setCursor(QPixmap("images/tank/cursor.png"));
    this->ping = ping;
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

void FrmMain::on_otherDeath(QRectF rect, bool flak)
{
    if(!flak) {
        expAn.append(new ExpAnimation(rect));
        sound->playDeath();
    } else {
        rect.adjust(-7,-7,14,14);
        expAn.append(new ExpAnimation(rect));
        sound->playOwnHit(1);
    }
}

void FrmMain::on_shot(int type)
{
    sound->playShot(type);
}

void FrmMain::on_hit(Tank *t, QString dmg)
{
    Q_UNUSED(t);
    animations.append(Animation(dmg,aim->x()-5,aim->y()-15));
    dmg.replace("-","");
    dmg.replace("+","");
    maxDmg += dmg.toInt();
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
    ownTank->setSpawned(true);
    ownTank->getBody()->SetLinearVelocity(b2Vec2(0,0));
    transX = (ownTank->getRect().x()-940)*-1;
    transY = (ownTank->getRect().y()-520)*-1;
    //t_spawn->start(10);
}

void FrmMain::on_death()
{
    sound->playDeath();
    sAstep = 0;
    //scaleX = double(this->geometry().width()/double(width+936));
    //scaleY = double(this->geometry().height()/double(height+576));
    //t_death->start(10);
    transX = 468;
    transY = 288;
    maxDmg = 0;
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
    if(message=="") return;
    message.replace("&0","ß");
    message.replace("&1","ä");
    message.replace("&2","ö");
    message.replace("&3","ü");
    message.replace("&11","Ä");
    message.replace("&22","Ö");
    message.replace("&33","Ü");
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
    ownTank->setAngle(qRadiansToDegrees(atan2(aim->y()-ownTank->getRect().center().y(),
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
        this->showMaximized();
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
    Q_UNUSED(visible)
}

void FrmMain::on_newMap(QVector<Terrain*> lvlObjs)
{
    this->lvlObjs = lvlObjs;
}

void FrmMain::on_ownHit()
{
    int type = 0;
    if(ownTank->getVehicleID()==1) type = 1;
    sound->playOwnHit(type);
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

void FrmMain::on_msgBox(QString title, QString text)
{
    QMessageBox::information(this,title,text);
}

void FrmMain::on_resetMatch(int team)
{
    QString msg;
    if(team==ownTank->getTeam()) {
        msg = "DEIN TEAM HAT GEWONNEN!";
    } else {
        msg = "GEGNER TEAM HAT GEWONNEN!";
    }
    for(int i=0;i<30;i++) on_killMessage(msg);
    team1CP = 0;
    team2CP = 0;
    ownTank->setAll(-200,-200);
    ownTank->setSpawned(false);
    on_death();
}

void FrmMain::on_powerup(Powerup *tmp)
{
    powerups.append(tmp);
}

void FrmMain::on_delPowerup(int pos)
{
    if(pos==-1) {
        for(int i=0;i<powerups.size();i++) {
            delete powerups[i];
        }
        powerups.resize(0);
        return;
    }
    delete powerups[pos];
    powerups.removeAt(pos);
}

void FrmMain::drawPlayerScores(QPainter &p)
{
    p.setBrush(Qt::darkGray);
    QFont f = p.font();
    int size,diff,x;
    if(ownTank->isSpawned()) {
        size = 22;
        diff = 30;
        x = 1780;
    } else {
        size = 48;
        diff = 75;
        x = 3520;
    }
    f.setPointSize(size);
    p.setFont(f);
    int startX = 125;
    p.setOpacity(0.75);
    QVector <int> ownTeam;
    QVector <int> otherTeam;
    for(int i=0;i<tanks.size();i++) {
        if(tanks.at(i)->getTeam()==ownTank->getTeam()) {
            ownTeam.append(i);
        } else {
            otherTeam.append(i);
        }
    }
    for(int i=-1;i<ownTeam.size();i++) {
        QString text;
        int health;
        int maxHealth;
        int num;
        if(i==-1) {
            text = ownTank->getName();
            health = ownTank->getHealth();
            maxHealth = ownTank->getHealth(1);
        } else {
            num = ownTeam[i];
            text = tanks.at(num)->getName();
            health = tanks.at(num)->getHealth();
            maxHealth = tanks.at(num)->getHealth(1);
        }
        QFontMetrics m(f);
        QRect br = m.boundingRect(text);
        p.setPen(Qt::NoPen);
        p.setBrush(Qt::darkGray);
        bool ok = false;
        if(i==-1&&ownTank->isSpawned()) {
            ok = true;
        } else if(i>-1) {
            if(tanks.at(num)->isSpawned()) ok = true;
        }
        if(ok) {
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
        }
        int width;
        if(i==-1) {
            if(ownTank->isSpawned()) {
                width = br.width()*((double)health/maxHealth);
            } else {
                width = br.width();
            }
            p.drawRect(10,-diff+startX+(0*diff)+10,width,br.height()-6);
        } else {
            if(tanks.at(num)->isSpawned()) {
                width = br.width()*((double)health/maxHealth);
            } else {
                width = br.width();
            }
            if(tanks.at(num)->getTeam()==ownTank->getTeam()) {
                p.drawRect(10,(-diff+startX)+((i+1)*diff)+10,width,br.height()-6);
            }
        }
    }
    for(int i=0;i<otherTeam.size();i++) {
        QString text;
        int health;
        int maxHealth;
        int num;
        num = otherTeam[i];
        text = tanks.at(num)->getName();
        health = tanks.at(num)->getHealth();
        maxHealth = tanks.at(num)->getHealth(1);
        QFontMetrics m(f);
        QRect br = m.boundingRect(text);
        p.setPen(Qt::NoPen);
        p.setBrush(Qt::darkGray);
        bool ok = false;
        if(tanks.at(num)->isSpawned()) ok = true;
        if(ok) {
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
        }
        int width;
            if(tanks.at(num)->isSpawned()) {
                width = br.width()*((double)health/maxHealth);
            } else {
                width = br.width();
            }
            p.drawRect(x,(-diff+startX)+(i*diff)+10,width,br.height()-6);
    }
    p.setOpacity(1);
    p.setPen(Qt::black);
    for(int i=-1;i<ownTeam.size();i++) { //links
        QString text;
        if(i==-1) {
            text = ownTank->getName();
            p.drawText(10,startX+0*diff,text);
        } else {
            int num = ownTeam[i];
            text = tanks.at(num)->getName();
            p.drawText(10,startX+(i+1)*diff,text);
        }
    }
    for(int i=0;i<otherTeam.size();i++) { //rechts
        int num = otherTeam[i];
        QString text;
        text = tanks.at(num)->getName();
        p.drawText(x,startX+i*diff,text);
    }
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
        viewRect = QRect(ownTank->getBody()->GetPosition().x+(ownTank->getRect().width()/2)-1000,
                               ownTank->getBody()->GetPosition().y+(ownTank->getRect().height()/2)-600,2140,1310);
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
        transX = (ownTank->getBody()->GetPosition().x+(ownTank->getRect().width()/2)-940)*-1;
        transY = (ownTank->getBody()->GetPosition().y+(ownTank->getRect().height()/2)-520)*-1;
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
    painter.setFont(font);
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
        }
    }
    QRect src(0,0,2880,2160);
    painter.drawPixmap(src,map);
    for(int i=startPos;i<endPos;i++) {
        if(lvlObjs[i]->getRect().intersects(viewRect)) {
            if(lvlObjs[i]->getType()>0) {
                if(lvlObjs[i]->getType()==4) {
                    painter.setBrush(QColor(255,0,0,150));
                    if(lvlObjs[i]->getRect().x()<1000&&ownTank->getTeam()==1) painter.setBrush(QColor(0,255,0,150));
                    if(lvlObjs[i]->getRect().x()>1000&&ownTank->getTeam()==2) painter.setBrush(QColor(0,255,0,150));
                    painter.drawRect(lvlObjs[i]->getRect());
                }
            }
        }
    }
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    for(int i=0;i<bullets.size();i++) {
        if(bullets[i]->getEnabled()) {
            painter.drawEllipse(bullets[i]->get().center(),3,3);
        }
    }
    //painter.setPen(Qt::NoPen);
    for(int i=0;i<powerups.size();i++) {
        painter.drawPixmap(powerups[i]->getRect(),powerups[i]->getPixmap());
    }
    painter.setPen(Qt::NoPen);
    if(ownTank->isSpawned()||t_spawn->isActive()) ownTank->drawTank(painter,ownTank,true);
    for(int i=0;i<tanks.size();i++) {
        if(tanks.at(i)->getRect().intersects(viewRect)&&tanks.at(i)->getRect().x()>0&&!tanks.at(i)->isHidden()) {
            tanks.at(i)->drawTank(painter,ownTank,true);
        }
    }
    for(int i=0;i<expAn.size();i++) {
        int step = expAn[i]->getStep();
        QRectF rect = expAn[i]->getRect();
        painter.drawPixmap(rect,expAnPixmap[step],QRectF(0,0,100,100));
    }
    /*<DEBUG>
    QRect rr;
    painter.setBrush(Qt::red);
    for(b2Body* a = world->GetBodyList();a;a = a->GetNext()) {
        if(a!=ownTank->getBody()) {
            rr = QRect(a->GetPosition().x-(72/2),a->GetPosition().y-(72/2),72,72);
            painter.setPen(Qt::NoPen);
            painter.save();
            painter.translate(rr.center());
            painter.rotate(qRadiansToDegrees(a->GetAngle()));
            painter.translate(-rr.center());
            painter.drawRect(rr);
            painter.restore();
        }

    }
    //</Debug>*/
    painter.setPen(Qt::blue);
    QFont f = painter.font();
    f.setPointSize(20);
    painter.setFont(f);
    for(int i=0;i<animations.size();i++) {
        painter.drawText(QPoint(aim->x()-15,aim->y()-animations[i].getCount()-17),animations[i].getText());
    }
    /*-------------------------
    f = painter.font();
    f.setPointSize(18);
    painter.setFont(f);
    painter.setPen(Qt::black);
    for(int i=startPos;i<endPos;i++) {
        painter.drawText(QPoint(lvlObjs[i]->getRect().center()),QString::number(i));
    }
    //----------------------------*/
    painter.resetTransform();
    /*painter.setPen(Qt::red);
    painter.drawLine(0,540,1920,540);
    painter.drawLine(960,0,960,1080);*/
    painter.scale(scaleX,scaleY);
    painter.setPen(Qt::black);
    drawPlayerScores(painter);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    if(ownTank->isSpawned()) {
        //HUD
        if(ownTank->getVehicleID()!=1) {
            if(!ownTank->getShootmode()) {
                painter.drawPixmap(20,900,60,60,mode0);
                painter.drawPixmap(100,900,60,60,mode1g);
            } else {
                painter.drawPixmap(20,900,60,60,mode0g);
                painter.drawPixmap(100,900,60,60,mode1);
            }
        }
        int health = ownTank->getHealth();
        int maxHealth = ownTank->getHealth(1);
        int offset = 400;
        //painter.drawRect(18,998,404,66);
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
        QPen pen;
        pen.setWidth(3);
        painter.setPen(pen);
        painter.drawRect(20,1000,400*((double)health/maxHealth),50);
        f = painter.font();
        f.setPointSize(32);
        painter.setFont(f);
        painter.setPen(Qt::black);
        painter.drawText(190,1040,QString::number(health));
        //reload
        //painter.setPen(Qt::NoPen);
        painter.setPen(pen);
        painter.setBrush(Qt::darkGreen);
        painter.drawRect(20,1052,400*((double)currentReload/maxReload),10);
        //dmg log start
        f = painter.font();
        f.setPointSize(24);
        painter.setFont(f);
        painter.drawText(1400,1060,"HP: "+QString::number(maxDmg));
        //dmg log end
        //Minimap start
        painter.save();
        painter.setPen(Qt::NoPen);
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
            if(tanks.at(i)->getTeam()==ownTank->getTeam()) {
                painter.setBrush(QColor(0,255,0));
            } else {
                painter.setBrush(QColor(255,0,0));
            }
            QRectF rect = tanks.at(i)->getRect();
            if(rect.x()>0&&!tanks.at(i)->isHidden()) {
                painter.drawRect(rect);
            }
        }
        painter.setOpacity(1);
        painter.restore();
        QRect arc(mpos->x()-35,mpos->y()-35,70,70);
        QPen pen2(Qt::black,7);
        painter.setPen(pen2);
        int startAngle = 90;
        int stopAngle = 0-(((double)currentReload/maxReload)*360);
        painter.drawArc(arc,startAngle*16,stopAngle*16);
        double remaining = maxReload-currentReload;
        if(remaining==-1) remaining = maxReload;
        painter.drawText(mpos->x()+50,mpos->y()+40,QString::number(remaining/1000,'f',2)+"s");
        if(!ownTank->getRect().intersects(QRect(0,0,2880,2160))) {
            painter.setPen(Qt::red);
            f = painter.font();
            f.setPointSize(64);
            painter.setFont(f);
            painter.drawText(500,700,"Zurück zum Spielfeld!");
        }

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
        f.setPointSize(150);
        painter.setFont(f);
        painter.setPen(Qt::black);
        if(ownTank->getType()) {
            painter.drawText(1200,230,"SELECT SPAWN");
        } else {
            painter.drawText(1200,230,"SELECT TANK");
        }
        painter.drawPixmap(500,2490,300,216,tanksMenu);
        int id = ownTank->getType();
        if(id) painter.drawPixmap(900,2490,200,200,classIcons[id-1]);
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
            size = 24;
            x = 20;
            y = 750;
            space = 30;
        } else {
            size = 36;
            x = 30;
            y = 1890;
            space = 75;
        }
        f.setPointSize(size);
        QFontMetrics m(f);
        painter.setFont(f);
        for(int i=chat.size();i>0;i--) {
            QString text = chat[chat.size()-i];
            QRect br = m.boundingRect(text);
            painter.setPen(Qt::NoPen);
            painter.setBrush(Qt::darkGray);
            painter.setOpacity(0.75);
            if(ownTank->isSpawned()) {
                painter.drawRect(x,y-52-((i-1)*space),br.width(),br.height());
            } else {
                painter.drawRect(x,y-114-((i-1)*space),br.width(),br.height());
            }
            painter.setOpacity(1.0);
            if(text.contains(ownTank->getName()+": ")) {
                painter.setPen(Qt::black);
            } else if(text.contains("*SERVER: ")) {
                painter.setPen(Qt::darkRed);
            } else {
                painter.setPen(Qt::darkBlue);
            }
            painter.drawText(x, y-(i*space),text);
        }
    }
    f = painter.font();
    painter.setBrush(Qt::darkGray);
    QString text = "PING: "+QString::number(ping)+"ms";
    painter.setPen(Qt::NoPen);
    if(ownTank->isSpawned()) {
        f.setPointSize(14);
        QFontMetrics m(f);
        QRect br = m.boundingRect(text);
        painter.drawRect(10,30,br.width(),br.height());
    } else {
        f.setPointSize(32);
        QFontMetrics m(f);
        QRect br = m.boundingRect(text);
        painter.drawRect(10,15,br.width(),br.height());
    }
    painter.setFont(f);
    painter.setPen(Qt::black);
    painter.drawText(10,50,text);
    int barWidth = 600;
    int x;
    if(!ownTank->isSpawned()) {
        x = ((width+936)/2)-40;
    } else {
        x = (1920/2)-40;
    }
    painter.setPen(Qt::black);
    painter.drawPixmap(x,0,80,50,win);
    if(ownTank->getTeam()==1) {
        painter.setBrush(Qt::darkGreen);
    } else {
        painter.setBrush(Qt::darkRed);
    }
    painter.drawRect(x-600,0,barWidth*((double)team1CP/barWidth),50);
    if(ownTank->getTeam()==2) {
        painter.setBrush(Qt::darkGreen);
    } else {
        painter.setBrush(Qt::darkRed);
    }
    painter.drawRect(x+80+(600-barWidth*((double)team2CP/barWidth)),0,barWidth*((double)team2CP/barWidth),50);
}

void FrmMain::on_tmouse()
{
    QPoint m;
    m.setX(this->mapFromGlobal(QCursor::pos()).x()/scaleX);
    m.setY(this->mapFromGlobal(QCursor::pos()).y()/scaleY);
    mpos->setX(m.x());
    mpos->setY(m.y());
    if(ownTank->isSpawned()) {
        this->aim->setX(ownTank->getRect().x()+mpos->x()-900);
        this->aim->setY(ownTank->getRect().y()+mpos->y()-480);
    } else {
        this->aim->setX(mpos->x()-transX);
        this->aim->setY(mpos->y()-transY);
    }
}

void FrmMain::on_reload(int current, int max)
{
    currentReload = current;
    maxReload = max;
}

void FrmMain::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    //QCursor::setPos(mapToGlobal(QPoint(1280/2,720/2)));
}

void FrmMain::closeEvent(QCloseEvent *e)
{
    Q_UNUSED(e)
    worker->close();
    settings->hide();
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
    if(e->key()==Qt::Key_Escape) {
        if(!settings->isVisible()) {
            settings->show();
        }
    }
    if(e->key()==Qt::Key_1) {
        ownTank->setShootmode(0);
    } else if(e->key()==Qt::Key_2) {
        ownTank->setShootmode(1);
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
