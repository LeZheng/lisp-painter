#-------------------------------------------------
#
# Project created by QtCreator 2017-08-16T07:29:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lisp-painter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    lfilewidget.cpp

HEADERS  += mainwindow.h \
    lfilewidget.h

FORMS    += mainwindow.ui \
    lfilewidget.ui
