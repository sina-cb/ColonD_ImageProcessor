#-------------------------------------------------
#
# Project created by QtCreator 2015-01-08T17:52:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ColonD_ImageProcessor
TEMPLATE = app

INCLUDEPATH += /opt/ros/hydro/include/opencv\
            /opt/ros/hydro/include

LIBS += "/opt/ros/hydro/lib/*.so"

SOURCES += main.cpp\
        mainwindow.cpp \
    captureimage.cpp \
    projecdata.cpp

HEADERS  += mainwindow.h \
    captureimage.h \
    projecdata.h

FORMS    += mainwindow.ui
