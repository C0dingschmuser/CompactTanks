#include "sound.h"

Sound::Sound(QObject *parent) : QObject(parent)
{
    int max = 2;
    for(int i=0;i<max;i++) {
        QSoundEffect *tmp = new QSoundEffect();
        tmp->setSource(QUrl::fromLocalFile(":/sounds/gun/shot"+QString::number(i)+".wav"));
        shot.append(tmp);
    }
    ownHit = new QSoundEffect();
    death = new QSoundEffect();
    ownHit->setSource(QUrl::fromLocalFile(":/sounds/hit.wav"));
    death->setSource(QUrl::fromLocalFile(":/sounds/death.wav"));
}

void Sound::setVolume(double volume)
{
    for(int i=0;i<shot.size();i++) shot[i]->setVolume(volume);
    ownHit->setVolume(volume);
    death->setVolume(volume);
}

void Sound::playShot(int type)
{
    int pos;
    switch(type) {
        case 5: case 2:
            pos = 2;
        break;
        default:
            pos = 1;
        break;

    }
    pos--;
    shot[pos]->play();
}

void Sound::playOwnHit()
{
    ownHit->play();
}

void Sound::playDeath()
{
    death->play();
}
