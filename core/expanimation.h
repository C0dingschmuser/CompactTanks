#ifndef EXPANIMATION_H
#define EXPANIMATION_H
#include <QRect>

class ExpAnimation
{
private:
    int step;
    QRect rect;
public:
    ExpAnimation();
    ExpAnimation(QRect rect);
    QRect getRect();
    void setStep(int step);
    int getStep();
};

#endif // EXPANIMATION_H
