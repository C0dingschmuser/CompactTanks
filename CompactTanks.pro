#-------------------------------------------------
#
# Project created by QtCreator 2017-05-10T16:57:33
#
#-------------------------------------------------

QT       += core gui network

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
    ui/frmlogin.cpp

HEADERS  += ui/frmmain.h \
    core/tank.h \
    core/movement.h \
    core/network.h \
    core/terrain.h \
    core/bullet.h \
    core/shoot.h \
    core/weapon.h \
    core/worker.h \
    ui/frmlogin.h

FORMS    += ui/frmmain.ui \
    ui/frmlogin.ui

RESOURCES += \
    resources.qrc

RC_FILE = resources.rc
