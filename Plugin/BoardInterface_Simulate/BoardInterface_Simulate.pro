#-------------------------------------------------
#
# Project created by QtCreator 2015-10-08T10:25:12
#
#-------------------------------------------------

QT       += core

TARGET = BoardInterface_Simulate
TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH    += ../../Threads

DESTDIR = ../plugins

SOURCES += BoardInterface_Simulate.cpp

HEADERS += BoardInterface_Simulate.h
DISTFILES += BoardInterface_Simulate.json

unix {
    INCLUDEPATH += /usr/local/include/
    LIBS += -L/usr/local/lib/ -lmodbus
    target.path = /usr/lib
    INSTALLS += target
}
