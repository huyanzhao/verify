#-------------------------------------------------
#
# Project created by QtCreator 2019-08-23T10:50:24
#
#-------------------------------------------------

QT       += core gui \
            network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = verify
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    meteraddress.cpp \
    slotsconfig.cpp \
    voldataconfig.cpp \
    command.cpp \
    batchadd.cpp \
    qstringinthex.cpp \
    curdataconfig.cpp \
    testitem.cpp \
    currentitem.cpp

HEADERS  += mainwindow.h \
    meteraddress.h \
    slotsconfig.h \
    voldataconfig.h \
    command.h \
    batchadd.h \
    qstringinthex.h \
    curdataconfig.h \
    testitem.h \
    currentitem.h

FORMS    += mainwindow.ui \
    meteraddress.ui \
    slotsconfig.ui \
    voldataconfig.ui \
    batchadd.ui \
    curdataconfig.ui

RESOURCES += \
    image.qrc
