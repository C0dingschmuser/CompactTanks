#ifndef SOUND_H
#define SOUND_H

#include <QObject>
#include <QSoundEffect>
#include <QVector>

class Sound : public QObject
{
    Q_OBJECT
private:
    QVector <QSoundEffect*> shot;
    QSoundEffect *ownHit;
    QSoundEffect *death;
public:
    explicit Sound(QObject *parent = nullptr);
    void setVolume(double volume);
    void playShot(int type);
    void playOwnHit();
    void playDeath();
signals:

public slots:
};

#endif // SOUND_H
