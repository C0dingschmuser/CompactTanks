#ifndef FRMMAIN_H
#define FRMMAIN_H
#include <QOpenGLWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QInputDialog>
#include "windows.h"
#include <QTimer>
#include <QKeyEvent>
#include <QThread>
#include <QFontDatabase>
#include <QApplication>
#include <QMessageBox>
#include "core/bullet.h"
#include "core/tank.h"
#include "core/movement.h"
#include "core/network.h"
#include "core/terrain.h"
#include "core/shoot.h"
#include "core/worker.h"
#include "core/animation.h"
#include "core/expanimation.h"
#include "core/filedownloader.h"
#include "ui/frmlogin.h"

namespace Ui {
class FrmMain;
}

class FrmMain : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit FrmMain(QWidget *parent = 0);
    ~FrmMain();
private slots:
    void on_tdraw();
    void on_newPlayer(Tank *t);
    void on_delPlayer(int pos);
    void on_newlvlObj(Terrain *t);
    void on_newBullet(Bullet *b);
    void on_delBullet(int pos);
    void on_delObjs();
    void on_tchat();
    void on_chat(QString message);
    void on_disconnect();
    void on_tmessage();
    void on_killMessage(QString message);
    void on_tkillMessage();
    void on_kick();
    void on_fullscreen();
    void on_visible(bool visible);
    void on_tab();
    void on_connFail();
    void on_newMap(QVector<Terrain*> lvlObjs);
    void on_connectData(QString username, QString pw, double volume, int graphics, bool lowTexture);
    void on_connSuccess();
    void on_wrongData(int id);
    void on_shot(int type);
    void on_hit(Tank *t, QString dmg);
    void on_thit();
    void on_spawn();
    void on_tspawn();
    void on_death();
    void on_tdeath();
    void on_msgBox(QString title, QString text);
    void on_otherDeath(QRect rect);
    void on_tExpAn();
    void on_ttime();
    void on_ping(int ping);
    void on_teamCP(int team1cp, int team2cp);
    void on_resetMatch(int team);
    void on_download();
    void on_ownHit();

private:
    Ui::FrmMain *ui;
    Tank *ownTank;
    Worker *worker;
    QVector <Tank*> tanks;
    QVector <Terrain*> lvlObjs;
    QVector <Bullet*> bullets;
    QVector <int> capObjs;
    QVector <Animation> animations;
    QVector <QPixmap> expAnPixmap;
    QVector <QPixmap> classIcons;
    QVector <ExpAnimation*> expAn;
    QTimer *t_draw;
    QTimer *t_chat;
    QTimer *t_killMessage;
    QTimer *t_hit;
    QTimer *t_spawn;
    QTimer *t_death;
    QTimer *t_expAn;
    QTimer *t_time;
    QThread *workerThread;
    FileDownloader *updateDL;
    bool lowGraphics;
    bool chatActive;
    QPoint *aim;
    QPoint *mpos;
    QRect viewRect;
    QFont font;
    Sound *sound;
    int messageLength;
    int width;
    int height;
    int sAstep;
    int msgCount;
    int ping;
    int graphics;
    int team1CP;
    int team2CP;
    int maxDmg;
    double transX;
    double transY;
    double scaleX;
    double scaleY;
    bool bmessage;
    bool killMessage;
    bool fullscreen;
    bool tab;
    bool isConnected;
    QVector <QString> chat;
    QVector <QString> messages;
    QVector <QRect> spawns;
    QPixmap tree;
    QPixmap grass;
    QPixmap minimap;
    QPixmap grid;
    QPixmap sSpawn;
    QPixmap sCap;
    QPixmap tanksMenu;
    QPixmap win;
    QPixmap map;
    QPainter::PixmapFragment camera;
    QString version;
    bool contains(QString data, QString c);
    void closeEvent(QCloseEvent *e) override;
    void drawPlayerScores(QPainter &p);
    FrmLogin *login;
protected:
    void paintEvent(QPaintEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void leaveEvent(QEvent *event) override;
};

#endif // FRMMAIN_H
