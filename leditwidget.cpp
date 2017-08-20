#include "leditwidget.h"
#include "ui_leditwidget.h"

LEditWidget::LEditWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LEditWidget)
{
    ui->setupUi(this);
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
