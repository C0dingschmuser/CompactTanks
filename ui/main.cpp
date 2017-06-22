#include "frmmain.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSurfaceFormat f;
    //f.setSwapBehavior(QSurfaceFormat::SingleBuffer);
    f.setSwapInterval(0);
    QSurfaceFormat::setDefaultFormat(f);
    FrmMain w;
    w.show();
    //w.showFullScreen();

    return a.exec();
}
