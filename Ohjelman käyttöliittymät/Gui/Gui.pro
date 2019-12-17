QT       += core gui serialport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

LIBS += $$PWD\..\..\build\rfidDLL.dll
LIBS += $$PWD\..\..\build\pinDLL.dll
LIBS += $$PWD\..\..\build\databaseDLL.dll

SOURCES += \
    depositview.cpp \
    eventview.cpp \
    giftview.cpp \
    invoiceview.cpp \
    main.cpp \
    mainview.cpp \
    mainwindow.cpp \
    startview.cpp \
    summaryview.cpp \
    withdrawalview.cpp

HEADERS += \
    Constants.h \
    depositview.h \
    eventview.h \
    giftview.h \
    invoiceview.h \
    mainview.h \
    mainwindow.h \
    startview.h \
    summaryview.h \
    withdrawalview.h

FORMS += \
    depositview.ui \
    eventview.ui \
    giftview.ui \
    invoiceview.ui \
    mainview.ui \
    mainwindow.ui \
    startview.ui \
    summaryview.ui \
    withdrawalview.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD\..\..\headers
DESTDIR = $$PWD\..\..\build

