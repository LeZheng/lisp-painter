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
    QMenu * fileMenu;
    QAction * openAction;
    QAction * saveAction;
    QAction * createAction;
    QAction * runAction;
    LEditWidget * editWidget;
    LConsoleWidget * consoleWidget;
    QString path;

    //TODO
    void initToolBar();
    QToolBar * fontToolBar;
    QLabel * fontLabel1;
    QFontComboBox * fontComboBox;
    QLabel * fontLabel2;
    QComboBox * sizeComboBox;
    QToolButton * boldBtn;
    QToolButton * italicBtn;
    QToolButton * underlineBtn;
    QToolButton * colorBtn;

protected slots:
    void showFontComboBox(QString font);
    void showSizeSpinBox(QString value);
    void showBoldBtn();
    void showItalicBtn();
    void showUnderlineBtn();
    void showColorBtn();
    void showCurrentFormatChanged(const QTextCharFormat &fmt);

};

#endif // MAINWINDOW_H
