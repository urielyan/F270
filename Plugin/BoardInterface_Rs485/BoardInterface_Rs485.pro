#-------------------------------------------------
#
# Project created by QtCreator 2015-09-29T17:13:22
#
#-------------------------------------------------

QT       += core gui

TARGET = BoardInterface_Rs485
TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH    += ../../Threads

DESTDIR = ../plugins

SOURCES += boardinterface_rs485.cpp

HEADERS += boardinterface_rs485.h
DISTFILES += BoardInterface_Rs485.json

unix {
    INCLUDEPATH += /usr/local/include/
    LIBS += -L/usr/local/lib/ -lmodbus
    target.path = /usr/lib
    INSTALLS += target
}
