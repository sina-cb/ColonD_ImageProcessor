#-------------------------------------------------
#
# Project created by QtCreator 2015-01-08T17:52:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ColonD_ImageProcessor
TEMPLATE = app

INCLUDEPATH += /usr/include/opencv

LIBS += -L/opt/lib -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_ts -lopencv_video -lopencv_videostab
LIBS += -L/usr/lib -lutil -lboost_iostreams -lboost_system -lboost_filesystem -lboost_thread

SOURCES += main.cpp\
        mainwindow.cpp \
    projecdata.cpp \
    loadimage.cpp \
    captureimage.cpp \
    histogramutility.cpp \
    processor.cpp \
    pyramid.cpp \
    steganography.cpp

HEADERS  += mainwindow.h \
    projecdata.h \
    captureimage.h \
    loadimage.h \
    gnuplot-iostream.h \
    histogramutility.h \
    processor.h \
    pyramid.h \
    steganography.h

FORMS    += mainwindow.ui
