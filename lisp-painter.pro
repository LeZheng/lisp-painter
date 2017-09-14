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
    src/startupwidget.cpp \
    src/lispsymbolfactory.cpp \
    src/qgraphicsstaritem.cpp \
    src/qfloatdockwidget.cpp

HEADERS  += \
    src/graphicsselectscene.h \
    src/lconsolewidget.h \
    src/leditwidget.h \
    src/lfilewidget.h \
    src/lispsyntaxhighlighter.h \
    src/mainwindow.h \
    src/startupwidget.h \
    src/lispsymbolfactory.h \
    src/qgraphicsstaritem.h \
    src/qfloatdockwidget.h

FORMS    += \
    src/lconsolewidget.ui \
    src/leditwidget.ui \
    src/lfilewidget.ui \
    src/mainwindow.ui

DISTFILES += \
    LICENSE \
    README.md

RESOURCES += \
    lisp-painter.qrc
