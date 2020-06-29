#-------------------------------------------------
#
# Project created by QtCreator 2020-03-10T15:57:42
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NetClient
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    tcpsocket.cpp

HEADERS  += widget.h \
    tcpsocket.h

FORMS    += widget.ui

RESOURCES += \
    res.qrc
