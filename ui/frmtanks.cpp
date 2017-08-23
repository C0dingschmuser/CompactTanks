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
    name = "ERROR";
    dmg = 0;
    reload = 0;
    speed = 0;
    health = 0;
    width = 0;
    camo = 0;
    viewrange = 0;
    height = 0;
    barrelLength = 0;
    softTerrRes = 0;
    hardTerrRes = 0;
    treeTerrRes = 0;
    treeColl = 0;
    vel = 0;
    ui->lwTanks->setIconSize(QSize(128,128));
    ui->lwTanks->setViewMode(QListView::IconMode);
    ui->lwTanks->setMovement(QListView::Static);
    ui->lwTanks->addItem(new QListWidgetItem(QIcon(":/images/tank/tank1.png"),"Pz. IV",ui->lwTanks,1001));
    ui->lwTanks->addItem(new QListWidgetItem(QIcon(":/images/tank/tank2.png"),"WT auf Pz. IV",ui->lwTanks,1002));
    ui->lwTanks->addItem(new QListWidgetItem(QIcon(":/images/tank/tank3.png"),"Leopard",ui->lwTanks,1003));
    ui->lwTanks->addItem(new QListWidgetItem(QIcon(":/images/tank/tank4.png"),"Tiger II",ui->lwTanks,1004));
    ui->lwTanks->addItem(new QListWidgetItem(QIcon(":/images/tank/tank5.png"),"KV-2",ui->lwTanks,1005));
    ui->lwTanks->addItem(new QListWidgetItem(QIcon(":/images/tank/tank6.png"),"M4 Sherman",ui->lwTanks,1006));
    ui->lwTanks->addItem(new QListWidgetItem(QIcon(":/images/tank/tank7.png"),"Tiger",ui->lwTanks,1007));
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
        painter.drawRect(605,27,width*((double)dmg/1500),10);
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
        painter.drawRect(605,117,width*((double)health/5000),10);
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
        painter.drawRect(605,177,width*((double)dpm/50000),10);
        //camo
        painter.setPen(Qt::black);
        painter.drawText(QPoint(605,203),"Tarnung: "+QString::number(100-(((double)camo/2000)*100))+ "/" +
                         QString::number(100-((((double)camo*1.3)/2000)*100)));
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::darkGray);
        painter.drawRect(605,207,width,10);
        painter.setBrush(Qt::darkGreen);
        painter.drawRect(605,207,width*((2000-(double)camo)/2000),10);
        //Sichtweite
        painter.setPen(Qt::black);
        painter.drawText(QPoint(605,233),"Sichtweite: "+QString::number(viewrange/6)+"m");
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::darkGray);
        painter.drawRect(605,237,width,10);
        painter.setBrush(Qt::darkGreen);
        painter.drawRect(605,237,width*((double)viewrange/2000),10);
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
    this->camo = dbTanks[id-1]->getCamo();
    this->viewrange = dbTanks[id-1]->getViewrange();
    update();
}

void FrmTanks::on_btnChoose_clicked()
{
    ownTank->setType(id);
    ownTank->setData(id,speed,health,vel,reload,width,height,barrelLength,treeColl,camo,viewrange);
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
    this->camo = dbTanks[id-1]->getCamo();
    this->viewrange = dbTanks[id-1]->getViewrange();
    ownTank->setType(id);
    ownTank->setData(id,speed,health,vel,reload,width,height,barrelLength,treeColl,camo,viewrange);
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
