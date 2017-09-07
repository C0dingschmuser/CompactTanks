#ifndef SOUND_H
#define SOUND_H

#include <QObject>
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMediaContent>
#include <QVector>

class Sound : public QObject
{
    Q_OBJECT
private:
    QVector <QSoundEffect*> shot;
    QSoundEffect *ownHit;
    QSoundEffect *death;
    QSoundEffect *planeHit;
    QMediaPlayer *music;
    QMediaPlaylist *playlist;
public:
    explicit Sound(QObject *parent = nullptr);
    void setVolume(double volume);
    void setMusicVolume(int volume);
    void playShot(int type);
    void playOwnHit(int type=0);
    void playDeath();
    void playMusic();
signals:

public slots:
};

#endif // SOUND_H
