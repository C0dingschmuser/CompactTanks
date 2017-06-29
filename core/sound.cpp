#include "sound.h"

Sound::Sound(QObject *parent) : QObject(parent)
{
    shot = new QSoundEffect();
    shot->setSource(QUrl::fromLocalFile(":/sounds/shot.wav"));
}

void Sound::setVolume(double volume)
{
    shot->setVolume(volume);
}

void Sound::playShot()
{
    shot->play();
}
