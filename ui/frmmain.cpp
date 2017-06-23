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
    initializeGL();
    QString name = QInputDialog::getText(this,"Name","Name:");
    if(contains(name,"|#äöü.,-_<>")||name.length()>8||name=="") {
        QMessageBox box;
        box.setText("Falsche Eingabe!");
        QApplication::exit();
    }
    ui->lwInfo->setGeometry(this->geometry().width()*0.8,20,256,391);
    bmessage = false;
    tab = false;
    width = 2880;
    height = 2160;
    scaleX = 1.0;
    scaleY = 1.0;
    fullscreen = false;
    aim = new QPoint();
    mpos = new QPoint();
    ownTank = new Tank(QRect(-200,-200,40,40),name);
    worker = new Worker(ownTank,aim,width,height);
    t_draw = new QTimer();
    t_message = new QTimer();
    t_killMessage = new QTimer();
    t_draw->setTimerType(Qt::PreciseTimer);
    QThread *workerThread = new QThread();
    connect(t_draw,SIGNAL(timeout()),this,SLOT(on_tdraw()));
    connect(t_message,SIGNAL(timeout()),this,SLOT(on_tmessage()));
    connect(t_killMessage,SIGNAL(timeout()),this,SLOT(on_tkillMessage()));
    connect(worker,SIGNAL(newBullet(Bullet*)),this,SLOT(on_newBullet(Bullet*)));
    connect(worker,SIGNAL(newPlayer(Tank*)),this,SLOT(on_newPlayer(Tank*))); //bei neuem spieler aufrufen
    connect(worker,SIGNAL(delPlayer(int)),this,SLOT(on_delPlayer(int)));
    connect(worker,SIGNAL(newlvlObj(Terrain*)),this,SLOT(on_newlvlObj(Terrain*)));
    connect(worker,SIGNAL(delBullet(int)),this,SLOT(on_delBullet(int)));
    //connect(network,SIGNAL(syncBullet(int,int,int,int)),this,SLOT(on_syncBullet(int,int,int,int)));
    connect(worker,SIGNAL(delObjs()),this,SLOT(on_delObjs()));
    connect(worker,SIGNAL(disconnected()),this,SLOT(on_disconnect()));
    connect(worker,SIGNAL(message(QString,int)),this,SLOT(on_message(QString,int)));
    connect(worker,SIGNAL(killMessage(QString)),this,SLOT(on_killMessage(QString)));
    connect(worker,SIGNAL(kick()),this,SLOT(on_kick()));
    connect(worker,SIGNAL(visible(bool)),this,SLOT(on_visible(bool)));
    //connect(network,SIGNAL(capobj(int,int,int)),this,SLOT(on_capobj(int,int,int)));
    connect(worker,SIGNAL(fullscreen()),this,SLOT(on_fullscreen()));
    connect(worker,SIGNAL(tab()),this,SLOT(on_tab()));
    connect(worker,SIGNAL(connFail()),this,SLOT(on_connFail()));
    connect(worker,SIGNAL(newMap(QVector<Terrain*>)),this,SLOT(on_newMap(QVector<Terrain*>)));
    worker->moveToThread(workerThread);
    workerThread->start();
    this->setCursor(QPixmap(":/images/tank/cursor.png"));
    tree = QPixmap(":/images/area/obj0.png");
    grass = QPixmap(":/images/area/obj9.png");
    QFontDatabase d;
    d.addApplicationFont(":/font/Pixeled.ttf");
    font = d.font("Pixeled","Normal",12);
    t_draw->start(5);
    //t_bullet->start(5);
}

FrmMain::~FrmMain()
{
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

void FrmMain::on_connFail()
{
    qDebug()<<"a";
    QMessageBox::critical(this,"FEHLER","Keine Verbindung möglich!");
    exit(1);
}

void FrmMain::on_disconnect()
{
    QMessageBox::information(this,"FEHLER","Verbindung zum Server getrennt!");
    QApplication::exit();
}

void FrmMain::on_message(QString message, int length)
{
    this->messageText.append(message);
    this->messageLength = length;
    if(!t_message->isActive()) {
        bmessage = true;
        t_message->start(length*1000);
   }
}

void FrmMain::on_killMessage(QString message)
{
    ui->lwInfo->addItem(message);
    if(!t_killMessage->isActive()) {
        killMessage = true;
        t_killMessage->start(1000);
    }
}

void FrmMain::on_tkillMessage()
{
    if(ui->lwInfo->count()==0) {
        killMessage = false;
        t_killMessage->stop();
    } else {
        delete ui->lwInfo->takeItem(0);
            if(ui->lwInfo->count()==0) {
                killMessage = false;
                t_killMessage->stop();
            }

    }
}

void FrmMain::on_tmessage()
{
    if(messageText.size()==0) {
        bmessage = false;
        t_message->stop();
    } else {
        delete ui->lwInfo->item(0);
            if(ui->lwInfo->count()==0) {
                bmessage = false;
                t_message->stop();
            }

    }
}

void FrmMain::on_newPlayer(Tank *t)
{
    tanks.append(t);
}

void FrmMain::on_newlvlObj(Terrain *t)
{
    Terrain *te = t;
    lvlObjs.append(te);
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
    ui->lwInfo->setGeometry(this->geometry().width()*0.8,20,256,391);
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
    if(visible) {
        ui->lblStatus->setText("Status: Offen");
    } else {
        ui->lblStatus->setText("Status: Versteckt");
    }
}

void FrmMain::on_newMap(QVector<Terrain*> lvlObjs)
{
    qDebug()<<"load";
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

void FrmMain::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    /*qDebug()<<"--------";
    qDebug()<<ownTank->getRect().x();
    qDebug()<<ownTank->getRect().y();*/
    scaleX = double(this->geometry().width()/double(1920));
    scaleY = double(this->geometry().height()/double(1080));
    //qDebug()<<this->geometry().width();
    viewRect = QRect(ownTank->getRect().center().x()-960,
                           ownTank->getRect().center().y()-540,2100,1250);
    worker->setViewRect(viewRect);
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.scale(scaleX,scaleY);
    painter.translate((ownTank->getRect().x()-940)*-1,(ownTank->getRect().y()-520)*-1);
    QPoint m;
    m.setX(this->mapFromGlobal(QCursor::pos()).x()/scaleX);
    m.setY(this->mapFromGlobal(QCursor::pos()).y()/scaleY);
    //m = painter.transform().map(m);
    mpos->setX(m.x());
    mpos->setY(m.y());
    this->aim->setX(ownTank->getRect().x()+mpos->x()-940);
    this->aim->setY(ownTank->getRect().y()+mpos->y()-520);
    //QFont f = QFont("Fixedsys");
    //painter.setFont(f);
    painter.setFont(font);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    for(int i=2160+576;i>viewRect.y()-72;i-=72) {
        for(int a=2880+936;a>viewRect.x()-72;a-=72) {
            if(QRect(a,i,72,72).intersects(viewRect)) {
                painter.drawPixmap(a,i,72,72,grass);
                painter.drawPixmap(a,i,72,72,tree);
            }
        }
    }
    //painter.drawRect(viewRect.x(),viewRect.y(),2000,1100);
    //painter.drawRect(-600,-500,6000,3000);
    /*painter.setPen(QColor(0,110,0));
    painter.setBrush(QColor(0,110,0));
    painter.drawRect(0,0,width,height);*/
    painter.setPen(Qt::NoPen);
    for(int i=0;i<lvlObjs.size();i++) {
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
            if(lvlObjs[i]->getType()>0&&lvlObjs[i]->getType()<14) {
                painter.drawPixmap(lvlObjs[i]->getRect().x(),lvlObjs[i]->getRect().y(),72,72,lvlObjs[i]->getPixmap());
            }
            if(!lvlObjs[i]->getType()) {
                painter.drawPixmap(lvlObjs[i]->getRect(),lvlObjs[i]->getGrassPixmap());
            }
        }
    }
    ownTank->drawTank(painter,ownTank,true);
    for(int i=0;i<tanks.size();i++) {
        if(tanks[i]->getRect().intersects(viewRect)&&tanks[i]->getRect().x()>0) {
            tanks[i]->drawTank(painter,ownTank,true);
        }
    }
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    for(int i=0;i<bullets.size();i++) {
        if(bullets[i]->getEnabled()) {
            painter.drawEllipse(bullets[i]->get().center(),5,5);
        }
    }
    for(int i=0;i<lvlObjs.size();i++) {
        if(lvlObjs[i]->getRect().intersects(viewRect)) {
            if(!lvlObjs[i]->getType()) {
                painter.drawPixmap(lvlObjs[i]->getRect(),lvlObjs[i]->getPixmap());
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

    /*
    painter.setPen(Qt::darkGray);
    painter.setBrush(Qt::darkGray);
    painter.drawRect(-10,-10,width+20,10);
    painter.drawRect(-10,height,width+20,10);
    painter.drawRect(-10,-10,10,height+10);
    painter.drawRect(width,-10,10,height+10);
    painter.setBrush(QColor(25,25,112,100));
    */

    //painter.setBrush(QColor(255,255,0,50));
    painter.resetTransform();
    /*painter.setPen(Qt::red);
    painter.drawLine(0,540,1920,540);
    painter.drawLine(960,0,960,1080);*/
    painter.scale(scaleX,scaleY);
    painter.setPen(Qt::NoPen);
    //painter.drawRect(0,0,1920,1110);
    if(bmessage) {
        QFont f = painter.font();
        f.setPointSize(32);
        painter.setFont(f);
        painter.setPen(Qt::white);
        painter.setBrush(Qt::white);
        QFontMetrics m(f);
        QRect br = m.boundingRect(messageText.last());
        //QRect a = QRect(ownTank->getRect().x()-402,ownTank->getRect().y()+278,100,33);
        painter.drawRect(100,600,br.width()+2,br.height());
        painter.setPen(Qt::black);
        painter.drawText(100,640,messageText.last());
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
    //painter.drawRect(QRect(aim->x(),aim->y(),10,10));

}

void FrmMain::on_death()
{

}

void FrmMain::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    //QCursor::setPos(mapToGlobal(QPoint(1280/2,720/2)));
}

void FrmMain::keyPressEvent(QKeyEvent *e)
{
    worker->keyP(e);
}

void FrmMain::keyReleaseEvent(QKeyEvent *e)
{
    worker->keyR(e);
}

void FrmMain::mousePressEvent(QMouseEvent *e)
{
    worker->mPrs(e);
}

void FrmMain::mouseReleaseEvent(QMouseEvent *e)
{
    worker->mRls(e);
}
