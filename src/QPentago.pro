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
    Model/Board.cpp \
    Model/Referee.cpp \
    Model/serialization.cpp \
    Presenter/Game.cpp \
    Presenter/Player.cpp \
    View/graphicbutton.cpp \
    View/graphicmenu.cpp \
    view.cpp \
    View/graphicstone.cpp \
    View/graphicquadrant.cpp \
    View/graphicboard.cpp

HEADERS  += \
    Model/Board.h \
    Model/Referee.h \
    Model/serialization.h \
    Presenter/Game.h \
    Presenter/Player.h \
    iview.h \
    View/graphicbutton.h \
    View/graphicmenu.h \
    view.h \
    View/graphicstone.h \
    View/graphicquadrant.h \
    View/graphicboard.h

RESOURCES += \
    View/source.qrc
