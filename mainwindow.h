#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QTextEdit>
#include <QFile>
#include <QFileDialog>
#include "lfilewidget.h"
#include "lconsolewidget.h"
#include "leditwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QMenu * fileMenu;
    QAction * openAction;
    QAction * saveAction;
    QAction * createAction;
    LEditWidget * editWidget;
};

#endif // MAINWINDOW_H
