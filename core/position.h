#ifndef POSITION_H
#define POSITION_H

#include <QObject>
#include <QPointF>

class Position : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
private:
    QPointF pos;
public:
    explicit Position(QPointF pos,QObject *parent = nullptr);
    void setPos(QPointF pos);
signals:

public slots:
};

#endif // POSITION_H
