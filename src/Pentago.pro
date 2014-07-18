#-------------------------------------------------
#
# Project created by QtCreator 2014-05-13T17:19:03
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Pentago
TEMPLATE = app


SOURCES += main.cpp\
        glview.cpp \
    GLinterface/GLfontutils.cpp \
    GLinterface/GLbutton.cpp \
    GLinterface/GLmenu.cpp \
    GLinterface/GLradiogroup.cpp \
    GLinterface/pentagoboard.cpp \
    GLinterface/stone.cpp \
    GLinterface/GLtextedit.cpp \
    GLinterface/GLtextures.cpp \
    GLinterface/Glabel.cpp


HEADERS  += glview.h \
    GLinterface/GLfontutils.h \
    GLinterface/GLRectangleCoord.h \
    iview.h \
    tests.h \
    GLinterface/GLbutton.h \
    GLinterface/GLfontkeeper.h \
    GLinterface/GLfontkeeperbase.h \
    GLinterface/GLmenu.h \
    GLinterface/GLradiogroup.h \
    GLinterface/GLrenderobject.h \
    GLinterface/GLtextures.h \
    GLinterface/stone.h \
    GLinterface/GLtextedit.h \
    GLinterface/pentagoboard.h \
    GLinterface/GLlabel.h

RESOURCES += \
    GLinterface/GLgraphics.qrc

CONFIG += c++11

RC_FILE = pentago.rc

linux: LIBS += -lGLU

OTHER_FILES += \
    android/AndroidManifest.xml

android {
 DEFINES += HAVE_GLES=true
 LIBS += -lGLESv1_CM
}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
