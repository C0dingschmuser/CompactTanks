#ifndef EXPANIMATION_H
#define EXPANIMATION_H
#include <QRectF>

class ExpAnimation
{
private:
    int step;
    QRectF rect;
public:
    ExpAnimation();
    ExpAnimation(QRectF rect);
    QRectF getRect();
    void setStep(int step);
    int getStep();
};

#endif // EXPANIMATION_H
