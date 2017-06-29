#ifndef SHOOT_H
#define SHOOT_H

#include <QObject>
#include <QTimer>
#include <QMouseEvent>
#include <QPoint>
#include <QString>
#include "network.h"
#include "tank.h"
#include "weapon.h"
#include "sound.h"


class Shoot : public QObject
{
    Q_OBJECT
private slots:
    void on_tcool();
    void on_tmain();
private:
    bool isCool;
    bool click;
    Network *network;
    QTimer *t_cool;
    QTimer *t_main;
    Weapon *w;
    QPoint *mpos;
    Tank *t;
    Sound *sound;
public:
    explicit Shoot(Tank *t, Network *n, QPoint *aim,Sound *sound,QObject *parent = 0);
    void MousePressEvent(QMouseEvent *e);
    void MouseReleaseEvent(QMouseEvent *e);
    void setMpos(QPoint mpos);

signals:
    void newBullet(Bullet *b);
public slots:
};

#endif // SHOOT_H
