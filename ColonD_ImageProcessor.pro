#-------------------------------------------------
#
# Project created by QtCreator 2015-01-08T17:52:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ColonD_ImageProcessor
TEMPLATE = app

#QMAKE_CXXFLAGS += -Wall -Wextra -O0 -g -D_GLIBCXX_DEBUG

INCLUDEPATH += /opt/ros/hydro/include/opencv\
            /opt/ros/hydro/include

LIBS += "/opt/ros/hydro/lib/*.so"
LIBS +=  -L /usr/lib -lutil -lboost_iostreams -lboost_system -lboost_filesystem

SOURCES += main.cpp\
        mainwindow.cpp \
    projecdata.cpp \
    loadimage.cpp \
    captureimage.cpp \
    histogramutility.cpp \
    processor.cpp

HEADERS  += mainwindow.h \
    projecdata.h \
    captureimage.h \
    loadimage.h \
    gnuplot-iostream.h \
    histogramutility.h \
    processor.h

FORMS    += mainwindow.ui
