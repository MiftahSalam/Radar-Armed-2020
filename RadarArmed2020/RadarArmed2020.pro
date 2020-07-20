#-------------------------------------------------
#
# Project created by QtCreator 2017-07-31T10:27:03
#
#-------------------------------------------------

QT       += core gui opengl network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RadarAPP
TEMPLATE = app


DEFINES += QT_NO_DEBUG_OUTPUT
# disable debug output in release mode
CONFIG(release, debug|release): DEFINES += QT_NO_DEBUG_OUTPUT

SOURCES += main.cpp\
        mainwindow.cpp \
    framecontrol1.cpp \
    framecontrol2.cpp \
    framecontrol3.cpp \
    framecursor.cpp \
    frameosd.cpp \
    frametrackdisplay.cpp \
    frameadsb.cpp \
    radarwidget.cpp \
    gzdialog.cpp \
    dialogalarm.cpp \
    traildialog.cpp \
    adsbdialog.cpp \
    dialogradar.cpp \
    qtmosq.cpp

HEADERS  += mainwindow.h \
    framecontrol1.h \
    framecontrol2.h \
    framecontrol3.h \
    framecursor.h \
    frameosd.h \
    frametrackdisplay.h \
    frameadsb.h \
    glwidget.h \
    radarwidget.h \
    gzdialog.h \
    dialogalarm.h \
    traildialog.h \
    adsbdialog.h \
    dialogradar.h \
    qtmosq.h

FORMS    += mainwindow.ui \
    framecontrol1.ui \
    framecontrol2.ui \
    framecontrol3.ui \
    framecursor.ui \
    frameosd.ui \
    frametrackdisplay.ui \
    frameadsb.ui \
    gzdialog.ui \
    dialogalarm.ui \
    traildialog.ui \
    adsbdialog.ui \
    dialogradar.ui

unix:!macx: LIBS += -L/usr/local/lib/ -lmosquittopp

INCLUDEPATH += /usr/local/include
DEPENDPATH += /usr/local/include


unix:!macx: LIBS += -L/usr/lib/ -lradarengine-armed

INCLUDEPATH += /usr/include/radar-engine-armed
DEPENDPATH += /usr/include/radar-engine-armed

unix:!macx: PRE_TARGETDEPS += /usr/lib/libradarengine-armed.a
