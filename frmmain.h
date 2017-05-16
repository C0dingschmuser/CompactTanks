#ifndef FRMMAIN_H
#define FRMMAIN_H
#include <QOpenGLWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QInputDialog>
#include <QTimer>
#include <QMessageBox>
#include "bullet.h"
#include "tank.h"
#include "movement.h"
#include "network.h"
#include "terrain.h"
#include "shoot.h"

namespace Ui {
class FrmMain;
}

class FrmMain : public QOpenGLWidget
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
    void on_syncBullet(int pos,int x,int y, int elapsed);
    void on_dellObjs();
    void on_disconnect();
    void on_tbullet();
    void on_death();
private:
    Ui::FrmMain *ui;
    Tank *ownTank;
    QVector <Tank*> tanks;
    QVector <Terrain*> lvlObjs;
    QVector <Bullet*> bullets;
    QTimer *t_draw;
    QTimer *t_bullet;
    Movement *move;
    Network *network;
    Shoot *shoot;
    QPoint *aim;
    QPoint *mpos;
    bool contains(QString data, QString c);
protected:
    void paintEvent(QPaintEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
};

#endif // FRMMAIN_H
