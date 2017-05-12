#ifndef FRMMAIN_H
#define FRMMAIN_H
#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QInputDialog>
#include <QTimer>
#include "bullet.h"
#include "tank.h"
#include "movement.h"
#include "network.h"
#include "terrain.h"
#include "shoot.h"

namespace Ui {
class FrmMain;
}

class FrmMain : public QWidget
{
    Q_OBJECT

public:
    explicit FrmMain(QWidget *parent = 0);
    ~FrmMain();
private slots:
    void on_tdraw();
    void on_newPlayer(Tank *t);
    void on_delPlayer(int pos);
    void on_newlvlObj(int x,int y,int w,int h);
    void on_newBullet(Bullet *b);
    void on_delBullet(int pos);
    void on_syncBullet(int pos,int x,int y);
private:
    Ui::FrmMain *ui;
    Tank *ownTank;
    QVector <Tank*> tanks;
    QVector <Terrain*> lvlObjs;
    QVector <Bullet*> bullets;
    QTimer *t_draw;
    Movement *move;
    Network *network;
    Shoot *shoot;
    QPoint *mpos;
protected:
    void paintEvent(QPaintEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
};

#endif // FRMMAIN_H
