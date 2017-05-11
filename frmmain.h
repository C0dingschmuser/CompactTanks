#ifndef FRMMAIN_H
#define FRMMAIN_H
#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QInputDialog>
#include <QTimer>
#include "tank.h"
#include "movement.h"
#include "network.h"
#include "terrain.h"

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
private:
    Ui::FrmMain *ui;
    Tank *ownTank;
    QVector <Tank*> tanks;
    QVector <Terrain*> lvlObjs;
    QTimer *t_draw;
    QTimer *t_moveW;
    QTimer *t_moveA;
    QTimer *t_moveS;
    QTimer *t_moveD;
    Movement *move;
    Network *network;
protected:
    void paintEvent(QPaintEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;
};

#endif // FRMMAIN_H
