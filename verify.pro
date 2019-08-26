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
    meteraddress.cpp

HEADERS  += mainwindow.h \
    meteraddress.h

FORMS    += mainwindow.ui \
    meteraddress.ui

RESOURCES += \
    image.qrc

OTHER_FILES +=


