#ifndef ANIMATION_H
#define ANIMATION_H
#include <QString>
#include <QPoint>

class Animation
{
private:
    QString text;
    int x;
    int y;
    int count;
public:
    Animation(QString text, int x, int y);
    Animation();
    void setCoords(int x,int y);
    int getCount();
    QPoint getCoords();
    QString getText();
};

#endif // ANIMATION_H
