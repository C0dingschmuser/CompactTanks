#include "frmmain.h"
#include "ui_frmmain.h"

FrmMain::FrmMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmMain)
{
    ui->setupUi(this);
    QString name = QInputDialog::getText(this,"Name","Name:");
    ownTank = new Tank(QRect(50,50,20,20),name);
    move = new Movement(ownTank);
    network = new Network(ownTank,tanks,QHostAddress::LocalHost); //ip noch ändern!
    t_draw = new QTimer();
    connect(t_draw,SIGNAL(timeout()),this,SLOT(on_tdraw()));
    connect(network,SIGNAL(newPlayer(Tank*)),this,SLOT(on_newPlayer(Tank*))); //bei neuem spieler aufrufen
    connect(network,SIGNAL(delPlayer(int)),this,SLOT(on_delPlayer(int)));
    connect(network,SIGNAL(newlvlObj(int,int,int,int)),this,SLOT(on_newlvlObj(int,int,int,int)));
    t_draw->start(10);
}

FrmMain::~FrmMain()
{
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

void FrmMain::on_delPlayer(int pos)
{
    tanks.removeAt(pos);
}

void FrmMain::on_tdraw()
{
    //qDebug()<<tanks.size();
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
