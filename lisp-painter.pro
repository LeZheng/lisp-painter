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
    src/graphicsselectscene.h \
    src/lconsolewidget.h \
    src/leditwidget.h \
    src/lfilewidget.h \
    src/lispsyntaxhighlighter.h \
    src/mainwindow.h \
    src/startupwidget.h

FORMS    += \
    src/lconsolewidget.ui \
    src/leditwidget.ui \
    src/lfilewidget.ui \
    src/mainwindow.ui \
    src/startupwidget.ui

DISTFILES += \
    LICENSE \
    README.md

RESOURCES += \
    lisp-painter.qrc
