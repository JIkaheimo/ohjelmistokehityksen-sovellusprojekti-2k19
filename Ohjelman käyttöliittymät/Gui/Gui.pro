QT       += core gui serialport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

LIBS += $$PWD\..\..\build\rfidDLL.dll

SOURCES += \
    databasedll.cpp \
    depositview.cpp \
    eventview.cpp \
    main.cpp \
    mainview.cpp \
    mainwindow.cpp \
    pindialog.cpp \
    startview.cpp \
    withdrawalview.cpp

HEADERS += \
    databasedll.h \
    depositview.h \
    eventview.h \
    mainview.h \
    mainwindow.h \
    pindialog.h \
    startview.h \
    withdrawalview.h

FORMS += \
    depositview.ui \
    eventview.ui \
    mainview.ui \
    mainwindow.ui \
    pindialog.ui \
    startview.ui \
    withdrawalview.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD\..\..\headers
DESTDIR = $$PWD\..\..\build

