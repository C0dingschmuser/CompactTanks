#include "frmtanks.h"
#include "ui_frmtanks.h"

FrmTanks::FrmTanks(QFont f,Tank *ownTank, QOpenGLWidget *parent) :
    QOpenGLWidget(parent),
    ui(new Ui::FrmTanks)
{
    ui->setupUi(this);
    f.setPointSize(10);
    font = f;
    this->ownTank = ownTank;
    id = 0;
    ui->lwTanks->setIconSize(QSize(128,128));
    ui->lwTanks->setViewMode(QListView::IconMode);
    ui->lwTanks->setMovement(QListView::Static);
    ui->lwTanks->addItem(new QListWidgetItem(QIcon(":/images/tank/tank1.png"),"Medium",ui->lwTanks,1001));
    ui->lwTanks->addItem(new QListWidgetItem(QIcon(":/images/tank/tank2.png"),"Jagdpanzer",ui->lwTanks,1002));
    ui->lwTanks->addItem(new QListWidgetItem(QIcon(":/images/tank/tank3.png"),"Scout",ui->lwTanks,1003));
    ui->lwTanks->addItem(new QListWidgetItem(QIcon(":/images/tank/tank4.png"),"Heavy",ui->lwTanks,1004));
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
        int width = 240;
        painter.setPen(Qt::black);
        painter.setFont(font);
        painter.drawText(QPoint(605,23),"Schaden: "+QString::number(dmg*0.7,'f',0)+"-"+QString::number(dmg*1.3,'f',0)+"HP");
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::darkGray);
        painter.drawRect(605,27,width,10);
        painter.setBrush(Qt::darkGreen);
        painter.drawRect(605,27,width*((double)dmg/100),10);
        //reload
        painter.setPen(Qt::black);
        painter.drawText(QPoint(605,53),"Nachladezeit: "+QString::number(reload/(double)1000,'f',2)+"s");
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::darkGray);
        painter.drawRect(605,57,width,10);
        painter.setBrush(Qt::darkGreen);
        painter.drawRect(605,57,width*((5000-(double)reload)/5000),10);
        //speed
        int val = calcSpeed(speed);
        painter.setPen(Qt::black);
        painter.drawText(QPoint(605,83),"Geschw.: "+QString::number(val)+"Km/h");
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::darkGray);
        painter.drawRect(605,87,width,10);
        painter.setBrush(Qt::darkGreen);
        painter.drawRect(605,87,width*((double)val/100),10);
        //health
        painter.setPen(Qt::black);
        painter.drawText(QPoint(605,113),"Leben: "+QString::number(health)+"HP");
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::darkGray);
        painter.drawRect(605,117,width,10);
        painter.setBrush(Qt::darkGreen);
        painter.drawRect(605,117,width*((double)health/500),10);
        //vel
        painter.setPen(Qt::black);
        painter.drawText(QPoint(605,143),"Projektilgeschw.: "+QString::number(vel*6)+"m/s");
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::darkGray);
        painter.drawRect(605,147,width,10);
        painter.setBrush(Qt::darkGreen);
        painter.drawRect(605,147,width*((double)vel/500),10);
        //dpm
        int dpm = dmg*(60000/(double)reload);
        painter.setPen(Qt::black);
        painter.drawText(QPoint(605,173),"Schaden pro Min.: "+QString::number(dpm)+"HP");
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::darkGray);
        painter.drawRect(605,177,width,10);
        painter.setBrush(Qt::darkGreen);
        painter.drawRect(605,177,width*((double)dpm/5000),10);
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

double FrmTanks::calcSpeed(int value)
{
    value*=2;
    double p1 = 1/(double)120;
    double p2 = (value-190)*(value-190);
    double p3 = 235/(double)6;
    double num = p1*p2+p3;
    return num;
}
