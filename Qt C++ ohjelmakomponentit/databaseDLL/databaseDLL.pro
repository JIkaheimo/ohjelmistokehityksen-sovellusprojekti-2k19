QT += sql
QT -= gui

TEMPLATE = lib

TARGET = databaseDLL

DEFINES += DATABASEDLL_LIBRARY

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    databasedll.cpp

HEADERS += \
    databaseDLL_global.h \
    databasedll.h

QT       += core gui serialport

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DESTDIR = $$PWD\..\..\build\

