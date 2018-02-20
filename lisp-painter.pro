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

#LIBS += -lsource-highlight-qt4


SOURCES += \
    src/graphicsselectscene.cpp \
    src/lconsolewidget.cpp \
    src/leditwidget.cpp \
    src/lfilewidget.cpp \
    src/lispsyntaxhighlighter.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/lispsymbolfactory.cpp \
    src/lactionmanager.cpp \
    src/ltoolswidget.cpp \
    src/lfloatdockwidget.cpp \
    src/lgraphicsstaritem.cpp \
    src/lstartupwidget.cpp \
    src/lsplitcopywidget.cpp \
    src/lispedit.cpp

HEADERS  += \
    src/graphicsselectscene.h \
    src/lconsolewidget.h \
    src/leditwidget.h \
    src/lfilewidget.h \
    src/lispsyntaxhighlighter.h \
    src/mainwindow.h \
    src/lispsymbolfactory.h \
    src/lactionmanager.h \
    src/ltoolswidget.h \
    src/lfloatdockwidget.h \
    src/lgraphicsstaritem.h \
    src/lstartupwidget.h \
    src/lsplitcopywidget.h \
    src/lispedit.h

FORMS    += \
    src/lconsolewidget.ui \
    src/leditwidget.ui \
    src/lfilewidget.ui \
    src/mainwindow.ui \
    src/ltoolswidget.ui

DISTFILES += \
    LICENSE \
    README.md

RESOURCES += \
    lisp-painter.qrc
