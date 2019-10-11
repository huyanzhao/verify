#-------------------------------------------------
#
# Project created by QtCreator 2019-08-23T10:50:24
#
#-------------------------------------------------

QT       += core gui \
            network \
            sql

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
    currentitem.cpp \
    qjson4/QJsonDocument.cpp \
    qjson4/QJsonArray.cpp \
    qjson4/QJsonObject.cpp \
    qjson4/QJsonParseError.cpp \
    qjson4/QJsonValueRef.cpp \
    qjson4/QJsonValue.cpp \
    qjson4/QJsonParser.cpp \
    mythread.cpp \
    about.cpp \
    paraminfo.cpp \
    platformbuild.cpp \
    dataitem.cpp

HEADERS  += mainwindow.h \
    meteraddress.h \
    slotsconfig.h \
    voldataconfig.h \
    command.h \
    batchadd.h \
    qstringinthex.h \
    curdataconfig.h \
    testitem.h \
    currentitem.h \
    qjson4/QJsonDocument.h \
    qjson4/QJsonArray.h \
    qjson4/QJsonObject.h \
    qjson4/QJsonParseError.h \
    qjson4/QJsonValueRef.h \
    qjson4/QJsonValue.h \
    qjson4/QJsonRoot.h \
    qjson4/QJsonParser.h \
    mythread.h \
    about.h \
    paraminfo.h \
    platformbuild.h \
    dataitem.h

FORMS    += mainwindow.ui \
    meteraddress.ui \
    slotsconfig.ui \
    voldataconfig.ui \
    batchadd.ui \
    curdataconfig.ui \
    about.ui \
    paraminfo.ui \
    platformbuild.ui

RESOURCES += \
    image.qrc
