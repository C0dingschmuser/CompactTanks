#include "sound.h"

Sound::Sound(QObject *parent) : QObject(parent)
{
    int max = 3;
    for(int i=0;i<max;i++) {
        QSoundEffect *tmp = new QSoundEffect();
        tmp->setSource(QUrl::fromLocalFile("sounds/gun/shot"+QString::number(i)+".wav"));
        shot.append(tmp);
    }
    ownHit = new QSoundEffect();
    death = new QSoundEffect();
    planeHit = new QSoundEffect();
    music = new QMediaPlayer();
    playlist = new QMediaPlaylist();
    ownHit->setSource(QUrl::fromLocalFile("sounds/hit.wav"));
    death->setSource(QUrl::fromLocalFile("sounds/death.wav"));
    planeHit->setSource(QUrl::fromLocalFile("sounds/hit_flak.wav"));
    playlist->addMedia(QMediaContent(QUrl::fromLocalFile("music/track1.mp3")));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    playlist->setCurrentIndex(1);
    music->setPlaylist(playlist);
}

void Sound::setVolume(double volume)
{
    for(int i=0;i<shot.size();i++) shot[i]->setVolume(volume);
    ownHit->setVolume(volume);
    death->setVolume(volume);
    planeHit->setVolume(volume);
}

void Sound::setMusicVolume(int volume)
{
    music->setVolume(volume);
}

void Sound::playShot(int type)
{
    int pos;
    switch(type) {
        case 5: case 2:
            pos = 2;
        break;
        case 9:
            pos = 3;
        break;
        default:
            pos = 1;
        break;

    }
    pos--;
    shot[pos]->play();
}

void Sound::playOwnHit(int type)
{
    if(!type) {
        ownHit->play();
    } else {
        planeHit->play();
    }
}

void Sound::playDeath()
{
    death->play();
}

void Sound::playMusic()
{
    music->play();
}
