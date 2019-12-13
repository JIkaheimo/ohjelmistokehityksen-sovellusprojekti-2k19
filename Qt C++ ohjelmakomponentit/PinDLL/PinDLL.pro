QT += gui core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
DEFINES += PINDLL_LIBRARY

CONFIG += c++11

TARGET = pinDLL

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    pindialog.cpp \
    pindll.cpp

HEADERS += \
    PinDLL_global.h \
    pindialog.h \
    pindll.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

DESTDIR = $$PWD\..\..\build\

FORMS += \
    pindialog.ui
