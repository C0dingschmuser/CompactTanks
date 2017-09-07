#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <QObject>
#include <QTimer>
#include <QKeyEvent>
#include <QThread>
#include <QDebug>
#include "tank.h"
#include "terrain.h"

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
    QVector<Terrain*>lvlObjs;
    QVector<Terrain*>collObjs;
    QVector<QRect>spawns;
    int width;
    int height;
    int first;
    int sPos;
    int ePos;
public:
    explicit Movement(Tank *t,int width,int height,QObject *parent = 0);
    ~Movement();
    Q_INVOKABLE void keyPressEvent(QVector<Terrain*>lvlObjs, int sPos, int ePos, int key);
    Q_INVOKABLE void keyReleaseEvent(int key);
    void setSpawns(QVector<QRect>spawns);
    Q_INVOKABLE void stop();
    void run(QThread *thread);


signals:
    void fullscreen();
    void tab();
public slots:
};

#endif // MOVEMENT_H
