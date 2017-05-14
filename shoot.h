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
public:
    explicit Shoot(Tank *t, Network *n, QObject *parent = 0);
    void MousePressEvent(QMouseEvent *e);
    void MouseReleaseEvent(QMouseEvent *e);
    void setMpos(QPoint mpos);

signals:
    void newBullet(Bullet *b);
public slots:
};

#endif // SHOOT_H
