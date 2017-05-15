#include "frmmain.h"
#include "ui_frmmain.h"

FrmMain::FrmMain(QWidget *parent) :
    QOpenGLWidget(parent),
    ui(new Ui::FrmMain)
{
    ui->setupUi(this);
    setUpdateBehavior(UpdateBehavior(PartialUpdate));
    initializeGL();
    QString name = QInputDialog::getText(this,"Name","Name:");
    aim = new QPoint();
    mpos = new QPoint();
    ownTank = new Tank(QRect(-200,-200,40,40),name);
    move = new Movement(ownTank);
    network = new Network(ownTank,tanks,QHostAddress("94.114.254.180")); //ip noch ändern!
    shoot = new Shoot(ownTank,network,aim);
    t_draw = new QTimer();
    t_bullet = new QTimer();
    connect(t_draw,SIGNAL(timeout()),this,SLOT(on_tdraw()));
    connect(t_bullet,SIGNAL(timeout()),this,SLOT(on_tbullet()));
    connect(network,SIGNAL(newPlayer(Tank*)),this,SLOT(on_newPlayer(Tank*))); //bei neuem spieler aufrufen
    connect(network,SIGNAL(delPlayer(int)),this,SLOT(on_delPlayer(int)));
    connect(network,SIGNAL(newlvlObj(int,int,int,int)),this,SLOT(on_newlvlObj(int,int,int,int)));
    connect(network,SIGNAL(newBullet(Bullet*)),this,SLOT(on_newBullet(Bullet*)));
    connect(network,SIGNAL(delBullet(int)),this,SLOT(on_delBullet(int)));
    connect(network,SIGNAL(syncBullet(int,int,int,int)),this,SLOT(on_syncBullet(int,int,int,int)));
    connect(network,SIGNAL(delObjs()),this,SLOT(on_dellObjs()));
    connect(network,SIGNAL(disconnect()),this,SLOT(on_disconnect()));
    t_draw->start(10);
    t_bullet->start(10);
}

FrmMain::~FrmMain()
{
    for(int i=0;i<tanks.size();i++) {
        delete tanks[i];
    }
    for(int i=0;i<lvlObjs.size();i++) {
        delete lvlObjs[i];
    }
    delete t_draw;
    delete ownTank;
    delete move;
    delete network;
    delete aim;
    delete mpos;
    delete ui;
}

void FrmMain::on_disconnect()
{
    QApplication::exit();
}

void FrmMain::on_newPlayer(Tank *t)
{
    tanks.append(t);
}

void FrmMain::on_newlvlObj(int x, int y, int w, int h)
{
    Terrain *t = new Terrain(x,y,w,h);
    lvlObjs.append(t);
}

void FrmMain::on_newBullet(Bullet *b)
{
    bullets.append(b);
}

void FrmMain::on_delBullet(int pos)
{
    if(bullets.size()-1>=pos) {
        bullets.removeAt(pos);
    }
}

void FrmMain::on_syncBullet(int pos, int x, int y, int elapsed)
{
    if(bullets.size()-1>=pos) {
        bullets[pos]->sync(x,y,elapsed);
    }
}

void FrmMain::on_delPlayer(int pos)
{
    tanks.removeAt(pos);
}

void FrmMain::on_dellObjs()
{
    for(int i=0;i<lvlObjs.size();i++) {
        delete lvlObjs[i];
    }
    lvlObjs.resize(0);
}

void FrmMain::on_tbullet()
{
    if(bullets.size()>0) {
        for(int i=0;i<bullets.size();i++) {
            bullets[i]->update();
        }
    }
}

void FrmMain::on_tdraw()
{
    update();
}

void FrmMain::paintEvent(QPaintEvent *e)
{
    int viewRange=ownTank->getViewRange();
    QRect viewRect = QRect(ownTank->getRect().center().x()-viewRange-10,ownTank->getRect().center().y()-viewRange-10,
                            (viewRange*2)+10,(viewRange*2)+10);
    QPainter painter(this);
    //painter.setRenderHint(QPainter::HighQualityAntialiasing);
    painter.translate((ownTank->getRect().x()-600)*-1,(ownTank->getRect().y()-400)*-1);
    this->mpos->setX(this->mapFromGlobal(QCursor::pos()).x());
    this->mpos->setY(this->mapFromGlobal(QCursor::pos()).y());
    this->aim->setX(ownTank->getRect().x()+mpos->x()-600);
    this->aim->setY(ownTank->getRect().y()+mpos->y()-400);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawRect(-400,-220,2000,2500);
    painter.setPen(Qt::white);
    painter.setBrush(Qt::white);
    painter.drawRect(0,0,1280,720);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    for(int i=0;i<bullets.size();i++) {
        //bullets[i]->update();
        //if(bullets[i]->get().intersects(viewRect)) {
            painter.drawEllipse(bullets[i]->get().center(),5,5);
        //}
    }
    ownTank->drawTank(painter);
    for(int i=0;i<tanks.size();i++) {
        //if(tanks[i]->getRect().intersects(viewRect)) {
            tanks[i]->drawTank(painter);
        //}
    }
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    for(int i=0;i<lvlObjs.size();i++) {
        //if(lvlObjs[i]->getRect().intersects(viewRect)) {
            painter.drawRect(lvlObjs[i]->getRect());
        //}
    }
    //painter.setBrush(Qt::transparent);
    //painter.drawRect(viewRect);
    QPainterPath path;
    QPainterPath inner;
    path.addRect(0,0,1280,720);
    inner.addEllipse(ownTank->getRect().center(),viewRange,viewRange);
    path = path.subtracted(inner);
    painter.fillPath(path,QBrush(QColor(100,100,100,200)));
    painter.setBrush(Qt::transparent);
    painter.drawEllipse(ownTank->getRect().center(),viewRange,viewRange);
    painter.setPen(Qt::red);
    painter.setBrush(Qt::red);
    painter.drawRect(-10,-10,1300,10);
    painter.drawRect(-10,720,1300,10);
    painter.drawRect(-10,-10,10,730);
    painter.drawRect(1280,-10,10,730);
    painter.drawRect(QRect(aim->x(),aim->y(),10,10));

}

void FrmMain::on_death()
{

}

void FrmMain::keyPressEvent(QKeyEvent *e)
{
    move->keyPressEvent(e);
}

void FrmMain::keyReleaseEvent(QKeyEvent *e)
{
    move->keyReleaseEvent(e);
}

void FrmMain::mousePressEvent(QMouseEvent *e)
{
    shoot->MousePressEvent(e);
}

void FrmMain::mouseReleaseEvent(QMouseEvent *e)
{
    shoot->MouseReleaseEvent(e);
}
