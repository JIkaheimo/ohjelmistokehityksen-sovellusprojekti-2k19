#-------------------------------------------------
#
# Project created by QtCreator 2019-11-26T11:12:46
#
#-------------------------------------------------

QT       += core gui serialport

TARGET = rfidDLL

TEMPLATE = lib

DEFINES += RFIDDLL_LIBRARY

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        rfiddll.cpp

HEADERS += \
        rfiddll.h \
        rfiddll_global.h  


unix {
    target.path = /usr/lib
    INSTALLS += target
}

DESTDIR = $$PWD\..\..\build\
