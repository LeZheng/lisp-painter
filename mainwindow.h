#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QTextEdit>
#include <QFile>
#include "lfilewidget.h"

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
    void open(QString path);
    void save(QString path);
    void create(QString path);

private:
    Ui::MainWindow *ui;
    QMenu * fileMenu;
    QAction * openAction;
    QAction * saveAction;
    QAction * createAction;
    QMap<QString,QTextEdit *> edits;
};

#endif // MAINWINDOW_H
