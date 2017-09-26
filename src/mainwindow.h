#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QTextEdit>
#include <QFile>
#include <QFileDialog>
#include <QLabel>
#include <QFontComboBox>
#include <QToolButton>
#include <QColorDialog>
#include <QToolBar>
#include "lfilewidget.h"
#include "lconsolewidget.h"
#include "leditwidget.h"
#include "qfloatdockwidget.h"
#include "ltoolswidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void currentCharFormatChanged(QTextCharFormat fmt);

private:
    Ui::MainWindow *ui;
    LEditWidget * editWidget;
    LConsoleWidget * consoleWidget;
    LToolsWidget * toolWidget;
    QString path;

    void initFontToolBar();
    void initBaseToolBar();
};

#endif // MAINWINDOW_H
