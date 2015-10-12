QT += core
QT += network
QT -= gui

TARGET = ser

CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    handler.cpp \
    server.cpp \
    member.cpp \
    message.cpp \
    room.cpp

HEADERS += handler.h \
    server.h \
    member.h \
    message.h \
    room.h
