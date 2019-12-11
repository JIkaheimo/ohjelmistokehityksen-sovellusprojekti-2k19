QT += sql
QT -= gui

TEMPLATE = lib

TARGET = databaseDLL

DEFINES += DATABASEDLL_LIBRARY

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    account.cpp \
    card.cpp \
    databasedll.cpp \
    event.cpp \
    table.cpp

HEADERS += \
    account.h \
    card.h \
    databaseDLL_global.h \
    databasedll.h \
    event.h \
    table.h

QT       += core gui serialport

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DESTDIR = $$PWD\..\..\build\

