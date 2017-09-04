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
    lfilewidget.cpp \
    leditwidget.cpp \
    lconsolewidget.cpp \
    graphicsselectscene.cpp \
    lispsyntaxhighlighter.cpp \
    startupwidget.cpp

HEADERS  += mainwindow.h \
    lfilewidget.h \
    leditwidget.h \
    lconsolewidget.h \
    graphicsselectscene.h \
    lispsyntaxhighlighter.h \
    startupwidget.h

FORMS    += mainwindow.ui \
    lfilewidget.ui \
    leditwidget.ui \
    lconsolewidget.ui \
    startupwidget.ui
