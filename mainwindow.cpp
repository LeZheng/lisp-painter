#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->editWidget = new LEditWidget(this);
    setCentralWidget(editWidget);

    LFileWidget * fw = new LFileWidget(this);
    QDockWidget * leftDock = new QDockWidget(this);
    leftDock->setWidget(fw);
    leftDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::LeftDockWidgetArea,leftDock);


    connect(fw,&LFileWidget::itemSelected,this->editWidget,&LEditWidget::open);

    LConsoleWidget * console = new LConsoleWidget(this);
    QDockWidget * bottomDock = new QDockWidget(this);
    bottomDock->setWidget(console);
    bottomDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::BottomDockWidgetArea,bottomDock);

    fileMenu = menuBar()->addMenu(tr("file"));
    openAction = new QAction(tr("open"),this);
    openAction->setShortcut(tr("Ctrl+O"));
    saveAction = new QAction(tr("save"),this);
    saveAction->setShortcut(tr("Ctrl+S"));
    createAction = new QAction(tr("new"),this);
    createAction->setShortcut(tr("Ctrl+N"));
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(createAction);
    connect(openAction,&QAction::triggered,
            [=](bool checked)
    {
        QString path = QFileDialog::getOpenFileName(this);
        editWidget->open(path);
    });
    connect(saveAction,&QAction::triggered,
            [=](bool checked)
    {
        QString path = this->ui->tabWidget->tabText(this->ui->tabWidget->currentIndex());
        editWidget->save(path);
    });
    connect(createAction,&QAction::triggered,
            [=](bool checked)
    {
        QString path = QFileDialog::getSaveFileName(this);
        editWidget->create(path);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
