#ifndef SOUND_H
#define SOUND_H

#include <QObject>
#include <QSoundEffect>

class Sound : public QObject
{
    Q_OBJECT
private:
    QSoundEffect *shot;
public:
    explicit Sound(QObject *parent = nullptr);
    void setVolume(double volume);
    void playShot();
signals:

public slots:
};

#endif // SOUND_H
