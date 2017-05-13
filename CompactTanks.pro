#-------------------------------------------------
#
# Project created by QtCreator 2017-05-10T16:57:33
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CompactTanks
TEMPLATE = app


SOURCES += main.cpp\
        frmmain.cpp \
    tank.cpp \
    movement.cpp \
    network.cpp \
    terrain.cpp \
    bullet.cpp \
    shoot.cpp \
    weapon.cpp

HEADERS  += frmmain.h \
    tank.h \
    movement.h \
    network.h \
    terrain.h \
    bullet.h \
    shoot.h \
    weapon.h

FORMS    += frmmain.ui

RESOURCES += \
    resources.qrc
