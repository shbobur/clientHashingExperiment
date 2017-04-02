#-------------------------------------------------
#
# Project created by QtCreator 2017-01-13T15:10:14
#
#-------------------------------------------------

QT       += core gui network

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ClientForExperiment
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    client.cpp

HEADERS  += widget.h \
    client.h

FORMS    += widget.ui
