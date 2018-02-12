#-------------------------------------------------
#
# Project created by QtCreator 2017-11-20T01:42:39
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = AvarKom
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    parser.cpp \
    presenter.cpp \
    avarkom.cpp \
    command.cpp

HEADERS  += mainwindow.h \
    parser.h \
    presenter.h \
    avarkom.h \
    command.h

FORMS    += mainwindow.ui
