#include "frmmain.h"
#include "ui_frmmain.h"

FrmMain::FrmMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmMain)
{
    ui->setupUi(this);
    QString name = QInputDialog::getText(this,"Name","Name:");
    mpos = new QPoint();
    ownTank = new Tank(QRect(50,50,20,20),name);
    move = new Movement(ownTank);
    network = new Network(ownTank,tanks,QHostAddress::LocalHost); //ip noch ändern!
    shoot = new Shoot(ownTank,network);
    t_draw = new QTimer();
    connect(t_draw,SIGNAL(timeout()),this,SLOT(on_tdraw()));
    connect(network,SIGNAL(newPlayer(Tank*)),this,SLOT(on_newPlayer(Tank*))); //bei neuem spieler aufrufen
    connect(network,SIGNAL(delPlayer(int)),this,SLOT(on_delPlayer(int)));
    connect(network,SIGNAL(newlvlObj(int,int,int,int)),this,SLOT(on_newlvlObj(int,int,int,int)));
    connect(network,SIGNAL(newBullet(Bullet*)),this,SLOT(on_newBullet(Bullet*)));
    connect(network,SIGNAL(delBullet(int)),this,SLOT(on_delBullet(int)));
    connect(network,SIGNAL(syncBullet(int,int,int)),this,SLOT(on_syncBullet(int,int,int)));
    t_draw->start(10);
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
    delete mpos;
    delete ui;
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

void FrmMain::on_syncBullet(int pos, int x, int y)
{
    if(bullets.size()-1>=pos) {
        bullets[pos]->sync(x,y);
    }
}

void FrmMain::on_delPlayer(int pos)
{
    tanks.removeAt(pos);
}

void FrmMain::on_tdraw()
{
    mpos->setX(this->mapFromGlobal(QCursor::pos()).x());
    mpos->setY(this->mapFromGlobal(QCursor::pos()).y());
    update();
}

void FrmMain::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawRect(ownTank->getRect());
    for(int i=0;i<tanks.size();i++) {
        painter.drawRect(tanks[i]->getRect());
    }
    for(int i=0;i<lvlObjs.size();i++) {
        painter.drawRect(lvlObjs[i]->getRect());
    }
    for(int i=0;i<bullets.size();i++) {
        painter.drawRect(bullets[i]->get());
        bullets[i]->update();
    }
    QRect viewRect;
    int viewRange=120;
    viewRect = QRect(ownTank->getRect().center().x()-viewRange-10,ownTank->getRect().center().y()-viewRange-10,
                            (viewRange*2)+10,(viewRange*2)+10);
    painter.drawRect(viewRect);

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
    shoot->MousePressEvent(e,mpos);
}

void FrmMain::mouseReleaseEvent(QMouseEvent *e)
{
    shoot->MouseReleaseEvent(e,mpos);
}
