#include "animation.h"

Animation::Animation()
{

}

Animation::Animation(QString text, int x, int y)
{
    this->text = text;
    this->x = x;
    this->y = y;
    count = 0;
}

void Animation::setCoords(int x, int y)
{
    this->x = x;
    this->y = y;
    count++;
}

int Animation::getCount()
{
    return count;
}

QPoint Animation::getCoords()
{
    return QPoint(x,y);
}

QString Animation::getText()
{
    return text;
}
