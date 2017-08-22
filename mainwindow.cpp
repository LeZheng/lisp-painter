#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setMinimumHeight(600);
    this->setMinimumWidth(800);

    this->editWidget = new LEditWidget(this);
    this->editWidget->setMinimumHeight(300);
    this->editWidget->setMinimumWidth(400);

    setCentralWidget(editWidget);

    LFileWidget * fw = new LFileWidget(this);
    QDockWidget * leftDock = new QDockWidget(this);
    leftDock->setWidget(fw);
    leftDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::LeftDockWidgetArea,leftDock);


    connect(fw,&LFileWidget::itemSelected,this->editWidget,&LEditWidget::open);

    consoleWidget = new LConsoleWidget(this);
    QDockWidget * bottomDock = new QDockWidget(this);
    bottomDock->setWidget(consoleWidget);
    bottomDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::BottomDockWidgetArea,bottomDock);

    fileMenu = menuBar()->addMenu(tr("file"));
    openAction = new QAction(tr("open"),this);
    openAction->setShortcut(tr("Ctrl+O"));
    saveAction = new QAction(tr("save"),this);
    saveAction->setShortcut(tr("Ctrl+S"));
    createAction = new QAction(tr("new"),this);
    createAction->setShortcut(tr("Ctrl+N"));
    runAction = new QAction(tr("run"),this);
    runAction->setShortcut(tr("Ctrl+R"));
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(createAction);
    fileMenu->addAction(runAction);
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
    connect(runAction,&QAction::triggered,
            [=](bool checked)
    {
        this->consoleWidget->processCmd("clisp " + path);
    });
    connect(this->editWidget,&LEditWidget::currentFileChanged,
            [=](QString path)
    {
        this->path = path;
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showFontComboBox(QString font)
{
    //TODO
}

void MainWindow::showSizeSpinBox(QString value)
{
    //TODO
}
void MainWindow::showBoldBtn()
{
    //TODO
}
void MainWindow::showItalicBtn()
{
    //TODO
}
void MainWindow::showUnderlineBtn()
{
    //TODO
}
void MainWindow::showColorBtn()
{
    //TODO
}
void MainWindow::showCurrentFormatChanged(const QTextCharFormat &fmt)
{
    //TODO
}
