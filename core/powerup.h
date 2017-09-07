#ifndef POWERUP_H
#define POWERUP_H
#include <QRect>
#include <QPixmap>

class Powerup
{
private:
    QRect rect;
    int type;
    int team;
    QPixmap pixmap;
public:
    Powerup();
    Powerup(QRect rect, int type, int team);
    QRect getRect();
    int getType();
    int getTeam();
    QPixmap getPixmap();
};

#endif // POWERUP_H
