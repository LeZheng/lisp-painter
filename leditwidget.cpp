#include "leditwidget.h"
#include "ui_leditwidget.h"
#include <QDebug>

LEditWidget::LEditWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LEditWidget)
{
    ui->setupUi(this);
    connect(this->ui->tabWidget,&QTabWidget::tabBarDoubleClicked,
            [=](int index)
    {
        qDebug() << this->ui->tabWidget->tabText(index);
        this->edits.remove(this->ui->tabWidget->tabText(index));
        QWidget * w = this->ui->tabWidget->widget(index);
        this->ui->tabWidget->removeTab(index);
        delete w;
    });

    connect(this->ui->tabWidget,&QTabWidget::currentChanged,
            [=](int index)
    {
        QString path = this->ui->tabWidget->tabText(index);
        emit currentFileChanged(path);
    });
}

LEditWidget::~LEditWidget()
{
    delete ui;
}

void LEditWidget::open(QString path)
{
    if(!edits.contains(path))
    {
        QFile f(path);
        if(f.open(QIODevice::ReadWrite))
        {
            QTextEdit * edit = new QTextEdit(this);
            edits[path] = edit;
            this->ui->tabWidget->addTab(edit,path);
            this->ui->tabWidget->setCurrentWidget(edit);
            edit->setText(QString(f.readAll().data()));
        }
        else
        {
            //TODO open failed
        }
    }
    else
    {
        int i = this->ui->tabWidget->indexOf(edits[path]);
        this->ui->tabWidget->setCurrentIndex(i);
    }
}

void LEditWidget::save(QString path)
{
    QTextEdit * edit = edits[path];
    if(edit != NULL){
        QFile f(path);
        if(f.open(QIODevice::WriteOnly))
        {
            f.write(edit->toPlainText().toUtf8());
            f.close();
        }
        else
        {
            //TODO
        }
    }
}

void LEditWidget::create(QString path)
{
    QFile f(path);
    if(f.open(QIODevice::ReadWrite))
    {
        QTextEdit * edit = new QTextEdit(this);
        edits[path] = edit;
        this->ui->tabWidget->addTab(edit,path);
    }
}
