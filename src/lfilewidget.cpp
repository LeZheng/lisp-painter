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
    this->ui->btnPrev->setIcon(QIcon(":/go-previous"));
    this->ui->btnNext->setIcon(QIcon(":/go-next"));
    connect(this->ui->fileTree,&QTreeView::doubleClicked,
            [=](const QModelIndex &index)
    {
        QString aPath = model->filePath(index);
        if(model->isDir(index))
        {
            this->ui->fileTree->setRootIndex(index);
            this->ui->cunrrentPath->setText(aPath);
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

    connect(this->ui->btnPrev,&QPushButton::clicked,
            [=]()
    {
        QModelIndex index = this->ui->fileTree->rootIndex().parent();
        if(index.isValid())
        {
            this->ui->fileTree->setRootIndex(index);
        }
    });
}

LFileWidget::~LFileWidget()
{
    delete ui;
}
