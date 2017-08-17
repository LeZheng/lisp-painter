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

    connect(fw,&LFileWidget::itemSelected,
            [=](QString path)
    {
        QFile f(path);
        if(f.open(QIODevice::ReadWrite)){
            QTextEdit * edit = new QTextEdit(this);
            this->ui->tabWidget->addTab(edit,path);
            edit->append(QString(f.readAll().data()));
        }else{
            //TODO open failed
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
