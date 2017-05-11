#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <QObject>
#include <QTimer>
#include <QKeyEvent>
#include "tank.h"

class Movement : public QObject
{
    Q_OBJECT
private slots:
    void on_tmove();
    void on_tmoveW();
    void on_tmoveA();
    void on_tmoveS();
    void on_tmoveD();
private:
    Tank *ownTank;
    QTimer *t_move;
    QTimer *t_moveW;
    QTimer *t_moveA;
    QTimer *t_moveS;
    QTimer *t_moveD;
public:
    explicit Movement(Tank *t,QObject *parent = 0);
    ~Movement();
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);


signals:

public slots:
};

#endif // MOVEMENT_H
