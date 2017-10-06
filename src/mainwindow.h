#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QTextEdit>
#include <QFileDialog>
#include <QLabel>
#include <QFontComboBox>
#include <QToolButton>
#include <QColorDialog>
#include <QToolBar>
#include <QApplication>
#include <QDesktopWidget>
#include "lfilewidget.h"
#include "lconsolewidget.h"
#include "leditwidget.h"
#include "lfloatdockwidget.h"
#include "ltoolswidget.h"
#include "lactionmanager.h"
#include "lsplitcopywidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void init();
signals:
    void currentCharFormatChanged(QTextCharFormat fmt);
protected:
    virtual void moveEvent(QMoveEvent *event);
private:
    Ui::MainWindow *ui;
    LFloatDockWidget * fdw;
    LFloatDockWidget * tdw;
    LFloatDockWidget * cdw;
    LEditWidget * editWidget;
    LConsoleWidget * consoleWidget;
    LToolsWidget * toolWidget;
    QString path;
    void initFloatDock();
    void initFontToolBar();
    void initBaseToolBar();
};

#endif // MAINWINDOW_H
