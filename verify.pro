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
    command.cpp

HEADERS  += mainwindow.h \
    meteraddress.h \
    slotsconfig.h \
    voldataconfig.h \
    command.h

FORMS    += mainwindow.ui \
    meteraddress.ui \
    slotsconfig.ui \
    voldataconfig.ui

RESOURCES += \
    image.qrc
