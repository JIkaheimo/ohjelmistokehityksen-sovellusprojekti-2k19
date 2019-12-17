QT += sql
QT -= gui

TEMPLATE = lib

TARGET = databaseDLL

DEFINES += DATABASEDLL_LIBRARY

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    account.cpp \
    card.cpp \
    customer.cpp \
    databasedll.cpp \
    event.cpp \
    invoice.cpp

HEADERS += \
    LibConstants.h \
    account.h \
    card.h \
    customer.h \
    databaseDLL_global.h \
    databasedll.h \
    event.h \
    invoice.h

QT       += core gui serialport

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DESTDIR = $$PWD\..\..\build\

