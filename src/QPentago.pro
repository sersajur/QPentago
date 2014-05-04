#-------------------------------------------------
#
# Project created by QtCreator 2014-05-02T20:01:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QPentago
TEMPLATE = app

CONFIG += c++11


SOURCES += main.cpp\
        mainwindow.cpp \
    Model/Board.cpp \
    Model/Referee.cpp \
    Model/serialization.cpp \
    Presenter/Game.cpp \
    Presenter/Player.cpp

HEADERS  += mainwindow.h \
    Model/Board.h \
    Model/Referee.h \
    Model/serialization.h \
    Presenter/Game.h \
    Presenter/Player.h

FORMS    += mainwindow.ui
