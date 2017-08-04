#include "expanimation.h"

ExpAnimation::ExpAnimation()
{
    this->step = 0;
}

ExpAnimation::ExpAnimation(QRect rect)
{
    this->rect = rect;
    this->step = 0;
}

void ExpAnimation::setStep(int step)
{
    this->step = step;
}

QRect ExpAnimation::getRect()
{
    return rect;
}

int ExpAnimation::getStep()
{
    return step;
}
