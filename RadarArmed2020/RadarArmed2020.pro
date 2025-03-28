#-------------------------------------------------
#
# Project created by QtCreator 2017-07-31T10:27:03
#
#-------------------------------------------------

QT       += core gui opengl network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RadarArmed20
TEMPLATE = app


#DEFINES += QT_NO_DEBUG_OUTPUT
# disable debug output in release mode
#CONFIG(release, debug|release): DEFINES += QT_NO_DEBUG_OUTPUT
CONFIG += console

SOURCES += main.cpp\
        mainwindow.cpp \
    framecontrol1.cpp \
    framecontrol2.cpp \
    framecontrol3.cpp \
    framecursor.cpp \
    frameosd.cpp \
    frametrackdisplay.cpp \
    radarwidget.cpp \
    gzdialog.cpp \
    dialogalarm.cpp \
    traildialog.cpp \
    dialogradar.cpp \
    frameradarstatus.cpp

HEADERS  += mainwindow.h \
    framecontrol1.h \
    framecontrol2.h \
    framecontrol3.h \
    framecursor.h \
    frameosd.h \
    frametrackdisplay.h \
    glwidget.h \
    radarwidget.h \
    gzdialog.h \
    dialogalarm.h \
    traildialog.h \
    dialogradar.h \
    frameradarstatus.h

FORMS    += mainwindow.ui \
    framecontrol1.ui \
    framecontrol2.ui \
    framecontrol3.ui \
    framecursor.ui \
    frameosd.ui \
    frametrackdisplay.ui \
    gzdialog.ui \
    dialogalarm.ui \
    traildialog.ui \
    dialogradar.ui \
    frameradarstatus.ui

win32 {
#    CONFIG += c++11 console
    message(Building for Windows)
    LIBS+= -lOpenGL32

    LIBS += -LC:\Users\WLR_Engine\ -lradarengine-armed
    INCLUDEPATH += C:\Users\WLR_Engine
    DEPENDPATH += C:\Users\WLR_Engine
}
else:unix {
    message(Building for Linux)

    LIBS += -L/usr/lib/ -lradarengine-armed

    INCLUDEPATH += /usr/include/radar-engine-armed
    DEPENDPATH += /usr/include/radar-engine-armed

    PRE_TARGETDEPS += /usr/lib/libradarengine-armed.a
}
else {
      message("Unknown configuration")
}

RESOURCES += \
    resource.qrc
