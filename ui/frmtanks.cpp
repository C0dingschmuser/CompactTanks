#include "frmtanks.h"
#include "ui_frmtanks.h"

FrmTanks::FrmTanks(QFont f,Tank *ownTank, QOpenGLWidget *parent) :
    QOpenGLWidget(parent),
    ui(new Ui::FrmTanks)
{
    ui->setupUi(this);
    f.setPointSize(8);
    font = f;
    this->ownTank = ownTank;
    id = 0;
    ui->lwTanks->setIconSize(QSize(128,128));
    ui->lwTanks->setViewMode(QListView::IconMode);
    ui->lwTanks->setMovement(QListView::Static);
    ui->lwTanks->addItem(new QListWidgetItem(QIcon(":/images/tank/tank1.png"),"Medium",ui->lwTanks,1001));
    ui->lwTanks->addItem(new QListWidgetItem(QIcon(":/images/tank/tank2.png"),"Jagdpanzer",ui->lwTanks,1002));
    ui->lwTanks->addItem(new QListWidgetItem(QIcon(":/images/tank/tank3.png"),"Scout",ui->lwTanks,1003));
}

FrmTanks::~FrmTanks()
{
    delete ui;
}

void FrmTanks::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter painter(this);
    painter.setBrush(Qt::white);
    painter.drawRect(0,0,this->size().width(),this->size().height());
    if(id) {
        //dmg
        painter.setPen(Qt::black);
        painter.setFont(font);
        painter.drawText(QPoint(605,23),"Schaden: "+QString::number(dmg));
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::darkGray);
        painter.drawRect(605,27,200,10);
        painter.setBrush(Qt::darkGreen);
        painter.drawRect(605,27,200*((double)dmg/100),10);
        //reload
        painter.setPen(Qt::black);
        painter.drawText(QPoint(605,53),"Nachladezeit: "+QString::number(reload/(double)1000,'f',2)+"s");
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::darkGray);
        painter.drawRect(605,57,200,10);
        painter.setBrush(Qt::darkGreen);
        painter.drawRect(605,57,200*((5000-(double)reload)/5000),10);
        //speed
        painter.setPen(Qt::black);
        painter.drawText(QPoint(605,83),"Geschwindigkeit: "+QString::number(speed));
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::darkGray);
        painter.drawRect(605,87,200,10);
        painter.setBrush(Qt::darkGreen);
        painter.drawRect(605,87,200*((double)speed/500),10);
        //health
        painter.setPen(Qt::black);
        painter.drawText(QPoint(605,113),"Leben: "+QString::number(health));
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::darkGray);
        painter.drawRect(605,117,200,10);
        painter.setBrush(Qt::darkGreen);
        painter.drawRect(605,117,200*((double)health/500),10);
        //vel
        painter.setPen(Qt::black);
        painter.drawText(QPoint(605,143),"Projektilgeschw.: "+QString::number(vel));
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::darkGray);
        painter.drawRect(605,147,200,10);
        painter.setBrush(Qt::darkGreen);
        painter.drawRect(605,147,200*((double)vel/500),10);
    }
}

void FrmTanks::setDB(QVector<dbTank *> dbTanks)
{
    this->dbTanks = dbTanks;
}

void FrmTanks::on_lwTanks_itemClicked(QListWidgetItem *item)
{
    if(id==item->type()-1000) return;
    int type = item->type()-1000;
    this->id = type;
    this->dmg = dbTanks[id-1]->getDmg();
    this->reload = dbTanks[id-1]->getReload();
    this->speed = dbTanks[id-1]->getSpeed();
    this->health = dbTanks[id-1]->getHealth();
    this->width = dbTanks[id-1]->getWidth();
    this->height = dbTanks[id-1]->getHeight();
    this->barrelLength = dbTanks[id-1]->getBarrelLength();
    this->softTerrRes = dbTanks[id-1]->getSoftTerrRes();
    this->hardTerrRes = dbTanks[id-1]->getHardTerrRes();
    this->treeTerrRes = dbTanks[id-1]->getTreeTerrRes();
    this->treeColl = dbTanks[id-1]->getTreeColl();
    this->vel = dbTanks[id-1]->getVel();
    update();
}

void FrmTanks::on_btnChoose_clicked()
{
    ownTank->setType(id);
    ownTank->setData(id,speed,health,vel,reload,width,height,barrelLength,treeColl);
    this->hide();
}

void FrmTanks::on_lwTanks_itemDoubleClicked(QListWidgetItem *item)
{
    int type = item->type()-1000;
    this->id = type;
    this->dmg = dbTanks[id-1]->getDmg();
    this->reload = dbTanks[id-1]->getReload();
    this->speed = dbTanks[id-1]->getSpeed();
    this->health = dbTanks[id-1]->getHealth();
    this->width = dbTanks[id-1]->getWidth();
    this->height = dbTanks[id-1]->getHeight();
    this->barrelLength = dbTanks[id-1]->getBarrelLength();
    this->softTerrRes = dbTanks[id-1]->getSoftTerrRes();
    this->hardTerrRes = dbTanks[id-1]->getHardTerrRes();
    this->treeTerrRes = dbTanks[id-1]->getTreeTerrRes();
    this->treeColl = dbTanks[id-1]->getTreeColl();
    this->vel = dbTanks[id-1]->getVel();
    ownTank->setType(id);
    ownTank->setData(id,speed,health,vel,reload,width,height,barrelLength,treeColl);
    this->hide();
}
