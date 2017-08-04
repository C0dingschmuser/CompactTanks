#ifndef FRMTANKS_H
#define FRMTANKS_H

#include <QOpenGLWidget>
#include <QPainter>
#include <QDebug>
#include <QString>
#include <QFont>
#include <QListWidgetItem>
#include <QVector>
#include "core/tank.h"
#include "core/dbtank.h"

namespace Ui {
class FrmTanks;
}

class FrmTanks : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit FrmTanks(QFont f,Tank *ownTank,QOpenGLWidget *parent = 0);
    void setDB(QVector <dbTank*> dbTanks);
    ~FrmTanks();

private:
    Ui::FrmTanks *ui;
    QVector <dbTank*> dbTanks;
    Tank *ownTank;
    QFont font;
    int id;
    QString name;
    int dmg;
    int reload;
    int speed;
    int health;
    int width;
    int height;
    int barrelLength;
    double softTerrRes;
    double hardTerrRes;
    double treeTerrRes;
    int treeColl;
    int vel;
protected:
    void paintEvent(QPaintEvent *e) override;
private slots:

    void on_lwTanks_itemClicked(QListWidgetItem *item);
    void on_btnChoose_clicked();
    void on_lwTanks_itemDoubleClicked(QListWidgetItem *item);
};

#endif // FRMTANKS_H
