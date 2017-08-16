#include "lfilewidget.h"
#include "ui_lfilewidget.h"
#include <QDebug>

LFileWidget::LFileWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LFileWidget)
{
    ui->setupUi(this);

    this->workPath = QDir::homePath();
    model = new QDirModel(this);

    this->ui->fileTree->setModel(model);

    connect(this->ui->fileTree,&QTreeView::doubleClicked,
            [=](const QModelIndex &index)
    {
        QModelIndex temp = index.parent();
        QString aPath = index.data().toString();
        while(temp.isValid())
        {
            QString p = temp.data().toString();
            if(p == "/")
            {
                aPath = p + aPath;
            }
            else
            {
                aPath = p + "/" + aPath;
            }

            temp = temp.parent();
        }
        qDebug() << aPath;

        if(model->isDir(index))
        {
            this->ui->fileTree->setRootIndex(index);
        }
        else
        {
            emit itemSelected(aPath);
        }

    });

    connect(this->ui->fileTree,&QTreeView::clicked,
            [=](const QModelIndex &index)
    {

    });
}

LFileWidget::~LFileWidget()
{
    delete ui;
}
