#-------------------------------------------------
#
# Project created by QtCreator 2017-06-19T23:09:53
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = InstrumentCluster
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
    cluster.cpp \
    fileparser.cpp \
    alphakalmanfilter.cpp \
    canreader.cpp \
    gpsreader.cpp \
    betakalmanfilter.cpp \
    rpmgauge.cpp \
    socgauge.cpp \
    laptiming.cpp \
    laptimepanel.cpp

HEADERS += cluster.h \
    fileparser.h \
    alphakalmanfilter.h \
    canreader.h \
    gpsreader.h \
    betakalmanfilter.h \
    rpmgauge.h \
    socgauge.h \
    laptiming.h \
    laptimepanel.h

FORMS   += cluster.ui


unix {
INCLUDEPATH += \
    /home/joe/raspi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/arm-linux-gnueabihf/libc/usr/include \
    /home/joe/raspi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/arm-linux-gnueabihf/libc/usr/lib \

LIBS += \
    -larmadillo
}

win32 {
INCLUDEPATH += \
    C:\Armadillo\include

LIBS += \
    -LC:\Armadillo\lib_win64 \
    -llapack_win64_MT \
    -lblas_win64_MT
}

target.path = /home/pi
INSTALLS += target
