#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QDockWidget * leftDock = new QDockWidget(this);
    leftDock->setWidget(new LFileWidget(this));
    addDockWidget(Qt::LeftDockWidgetArea,leftDock);


    setCentralWidget(new QTextEdit(this));
}

MainWindow::~MainWindow()
{
    delete ui;
}
