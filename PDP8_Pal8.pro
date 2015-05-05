#-------------------------------------------------
#
# Project created by QtCreator 2015-05-04T20:02:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PDP8_Pal8
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    codeeditor.cpp \
    pal8_highlighter.cpp

HEADERS  += mainwindow.h \
    codeeditor.h \
    pal8_highlighter.h

FORMS    += mainwindow.ui
