#ifndef FRMMAIN_H
#define FRMMAIN_H
#include <QOpenGLWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QInputDialog>
#include <QTimer>
#include <QKeyEvent>
#include <QThread>
#include <QApplication>
#include <QMessageBox>
#include "core/bullet.h"
#include "core/tank.h"
#include "core/movement.h"
#include "core/network.h"
#include "core/terrain.h"
#include "core/shoot.h"

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
    void on_newlvlObj(int x,int y,int w,int h,int type);
    void on_newBullet(Bullet *b);
    void on_delBullet(int pos);
    void on_syncBullet(int pos,int x,int y, int elapsed);
    void on_dellObjs();
    void on_disconnect();
    void on_tbullet();
    void on_death();
    void on_message(QString message, int length);
    void on_tmessage();
    void on_killMessage(QString message);
    void on_tkillMessage();
    void on_kick();
    void on_fullscreen();
    void on_visible(int visible);
    void on_tab();
    void on_capobj(int num,int owner,int cp);

private:
    Ui::FrmMain *ui;
    Tank *ownTank;
    QVector <Tank*> tanks;
    QVector <Terrain*> lvlObjs;
    QVector <Bullet*> bullets;
    QTimer *t_draw;
    QTimer *t_bullet;
    QTimer *t_message;
    QTimer *t_killMessage;
    Movement *move;
    Network *network;
    Shoot *shoot;
    QPoint *aim;
    QPoint *mpos;
    QRect viewRect;
    int messageLength;
    int width;
    int height;
    double scaleX;
    double scaleY;
    bool bmessage;
    bool killMessage;
    bool fullscreen;
    bool tab;
    QVector <QString> messageText;
    QVector <QString> killMessageText;
    bool contains(QString data, QString c);
protected:
    void paintEvent(QPaintEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void leaveEvent(QEvent *event) override;
};

#endif // FRMMAIN_H
