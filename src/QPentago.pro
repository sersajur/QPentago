#-------------------------------------------------
#
# Project created by QtCreator 2014-05-02T20:01:11
#
#-------------------------------------------------

QT       += core gui
QT       += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QPentago
TEMPLATE = app


SOURCES += main.cpp\
    graphicbutton.cpp \
    graphicmenu.cpp \
    view.cpp \
    graphicstone.cpp \
    graphicquadrant.cpp \
    graphicboard.cpp

HEADERS  += \
    iview.h \
    graphicbutton.h \
    graphicmenu.h \
    view.h \
    graphicstone.h \
    graphicquadrant.h \
    graphicboard.h

FORMS    +=

RESOURCES += \
    source.qrc
