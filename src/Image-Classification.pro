#-------------------------------------------------
#
# Project created by QtCreator 2016-01-22T13:38:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Image-Classification
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    sample.cpp \
    group.cpp \
    groupcontroller.cpp \
    classificator.cpp \
    trainer.cpp

HEADERS  += mainwindow.h \
    sample.h \
    group.h \
    groupcontroller.h \
    classificator.h \
    trainer.h

LIBS += -L/usr/local/lib \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_features2d \
    -lopencv_calib3d \
    -lopencv_flann \
    -lopencv_ml \
    -lopencv_nonfree
