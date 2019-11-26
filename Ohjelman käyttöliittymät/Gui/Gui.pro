QT       += core gui serialport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

LIBS += $$PWD\..\..\build\rfidDLL.dll

SOURCES += \
    aloitusview.cpp \
    databasedll.cpp \
    koontiview.cpp \
    main.cpp \
    mainwindow.cpp \
    pindialog.cpp

HEADERS += \
    aloitusview.h \
    databasedll.h \
    koontiview.h \
    mainwindow.h \
    pindialog.h

FORMS += \
    aloitusview.ui \
    koontiview.ui \
    mainwindow.ui \
    pindialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD\..\..\headers
DESTDIR = $$PWD\..\..\build

