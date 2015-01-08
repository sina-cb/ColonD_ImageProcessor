#-------------------------------------------------
#
# Project created by QtCreator 2015-01-08T16:19:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qt_OpenCV
TEMPLATE = app

INCLUDEPATH += /opt/ros/hydro/include/opencv\
            /opt/ros/hydro/include

LIBS += "/opt/ros/hydro/lib/*.so"


SOURCES += main.cpp\
        mainwindow.cpp \
    captureimage.cpp

HEADERS  += mainwindow.h \
    captureimage.h

FORMS    += mainwindow.ui
