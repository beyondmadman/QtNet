#-------------------------------------------------
#
# Project created by QtCreator 2020-03-10T12:42:02
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NetServer
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    tcpservice.cpp

HEADERS  += widget.h \
    tcpservice.h

FORMS    += widget.ui

RESOURCES += \
    res.qrc
