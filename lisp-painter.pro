#-------------------------------------------------
#
# Project created by QtCreator 2017-08-16T07:29:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lisp-painter
TEMPLATE = app

INCLUDEPATH += $$PWD/include

SOURCES += \
    src/graphicsselectscene.cpp \
    src/lconsolewidget.cpp \
    src/leditwidget.cpp \
    src/lfilewidget.cpp \
    src/lispsyntaxhighlighter.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/startupwidget.cpp

HEADERS  += \
    include/graphicsselectscene.h \
    include/lconsolewidget.h \
    include/leditwidget.h \
    include/lfilewidget.h \
    include/lispsyntaxhighlighter.h \
    include/mainwindow.h \
    include/startupwidget.h

FORMS    += \
    ui/lconsolewidget.ui \
    ui/leditwidget.ui \
    ui/lfilewidget.ui \
    ui/mainwindow.ui \
    ui/startupwidget.ui

DISTFILES += \
    LICENSE \
    README.md
