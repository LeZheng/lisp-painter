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
    this->ui->btnNext->setEnabled(false);
    this->ui->fileTree->header()->setVisible(false);
    this->ui->cunrrentPath->setReadOnly(true);
    for(int i = 1;i < this->model->columnCount();i++)
    {
        this->ui->fileTree->hideColumn(i);
    }
    connect(this->ui->fileTree,&QTreeView::doubleClicked,
            [=](const QModelIndex &index)
    {
        QString aPath = model->filePath(index);
        this->historyPathStack.clear();
        this->ui->btnNext->setEnabled(false);
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

    connect(this->ui->btnPrev,&QPushButton::clicked,
            [=]()
    {
        QModelIndex pIndex = this->ui->fileTree->rootIndex();
        QModelIndex index = pIndex.parent();
        if(index.isValid())
        {
            QString aPath = this->model->filePath(index);
            this->ui->cunrrentPath->setText(aPath);
            this->ui->fileTree->setRootIndex(index);
            this->historyPathStack.push(pIndex);
            this->ui->btnNext->setEnabled(true);
        }
    });

    connect(this->ui->btnNext,&QPushButton::clicked,
            [=]()
    {
        if(!this->historyPathStack.isEmpty())
        {
            QModelIndex index = this->historyPathStack.pop();
            QString aPath = this->model->filePath(index);
            this->ui->fileTree->setRootIndex(index);
            this->ui->cunrrentPath->setText(aPath);
        }
        if(this->historyPathStack.isEmpty())
            this->ui->btnNext->setEnabled(false);
    });
}

LFileWidget::~LFileWidget()
{
    delete ui;
}
