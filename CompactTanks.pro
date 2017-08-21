#-------------------------------------------------
#
# Project created by QtCreator 2017-05-10T16:57:33
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CompactTanks
TEMPLATE = app

SOURCES += ui/main.cpp\
        ui/frmmain.cpp \
    core/tank.cpp \
    core/movement.cpp \
    core/network.cpp \
    core/terrain.cpp \
    core/bullet.cpp \
    core/shoot.cpp \
    core/weapon.cpp \
    core/worker.cpp \
    ui/frmlogin.cpp \
    core/sound.cpp \
    core/animation.cpp \
    ui/frmtanks.cpp \
    core/dbtank.cpp \
    core/expanimation.cpp \
    core/filedownloader.cpp

HEADERS  += ui/frmmain.h \
    core/tank.h \
    core/movement.h \
    core/network.h \
    core/terrain.h \
    core/bullet.h \
    core/shoot.h \
    core/weapon.h \
    core/worker.h \
    ui/frmlogin.h \
    core/sound.h \
    core/animation.h \
    ui/frmtanks.h \
    core/dbtank.h \
    core/expanimation.h \
    core/filedownloader.h

FORMS    += ui/frmmain.ui \
    ui/frmlogin.ui \
    ui/frmtanks.ui

RESOURCES += \
    resources.qrc

RC_FILE = resources.rc
