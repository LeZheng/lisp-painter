#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->tabWidget->clear();

    LFileWidget * fw = new LFileWidget(this);
    QDockWidget * leftDock = new QDockWidget(this);
    leftDock->setWidget(fw);
    addDockWidget(Qt::LeftDockWidgetArea,leftDock);

    connect(fw,&LFileWidget::itemSelected,this,&MainWindow::open);

    fileMenu = menuBar()->addMenu(tr("file"));
    openAction = new QAction(tr("open"),this);
    saveAction = new QAction(tr("save"),this);
    createAction = new QAction(tr("new"),this);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(createAction);
    connect(openAction,&QAction::triggered,
            [=](bool checked)
    {
        if(checked)
        {
            //TODO
        }
    });
    connect(saveAction,&QAction::triggered,
            [=](bool checked)
    {
        if(checked)
        {
            QString path = this->ui->tabWidget->tabText(this->ui->tabWidget->currentIndex());
            save(path);
        }
    });
    connect(createAction,&QAction::triggered,
            [=](bool checked)
    {
        if(checked)
        {
            //TODO
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open(QString path)
{
    QFile f(path);
    if(f.open(QIODevice::ReadWrite)){
        QTextEdit * edit = new QTextEdit(this);
        edits[path] = edit;
        this->ui->tabWidget->addTab(edit,path);
        edit->setText(QString(f.readAll().data()));
    }else{
        //TODO open failed
    }
}

void MainWindow::save(QString path)
{
    QTextEdit * edit = edits[path];
    if(edit != NULL){
        QFile f(path);
        if(f.open(QIODevice::WriteOnly)){
            f.write(edit->toPlainText().toUtf8());
            f.close();
        }
    }
}

void MainWindow::create(QString path)
{
    QFile f(path);
    if(f.open(QIODevice::ReadWrite)){
        QTextEdit * edit = new QTextEdit(this);
        edits[path] = edit;
        this->ui->tabWidget->addTab(edit,path);
    }
}
