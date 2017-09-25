#include "ltoolswidget.h"
#include "ui_ltoolswidget.h"

LToolsWidget::LToolsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LToolsWidget)
{
    ui->setupUi(this);
    this->toolBox = this->ui->toolBox;
    this->toolBox->removeItem(0);
}

LToolsWidget::~LToolsWidget()
{
    delete ui;
}

QAction * LToolsWidget::addWidget(QString pageName,QWidget * w)
{
    if(pageMap.contains(pageName))
    {
        if(w->inherits("QToolBar"))
            return qobject_cast<QToolBar *>(w)->addWidget(w);
        return NULL;
    }
    else
    {
        QToolBar * toolbar = new QToolBar(this);
        this->toolBox->insertItem(0,toolbar,pageName);
        pageMap[pageName] = toolbar;
        return toolbar->addWidget(w);
    }
}

bool LToolsWidget::addPage(QString pageName,QWidget * page)
{
    if(pageMap.contains(pageName))
    {
        return false;
    }
    else
    {
        this->toolBox->insertItem(0,page,pageName);
        this->pageMap[pageName] = page;
        return true;
    }

}

void LToolsWidget::addAction(QString pageName,QAction * action)
{
    if(!pageMap.contains(pageName))
    {
        QToolBar * bar = new QToolBar(this);
        pageMap[pageName] = bar;
        this->toolBox->insertItem(0,bar,pageName);
    }
    pageMap[pageName]->addAction(action);
}

QAction * LToolsWidget::addAction(QString pageName,QString name,QIcon icon,QString shortcut,QString status)
{
    QAction * action = new QAction(icon,name,this);
    action->setShortcut(shortcut);
    action->setStatusTip(status);
    if(!pageMap.contains(pageName))
    {
        QToolBar * bar = new QToolBar(this);
        pageMap[pageName] = bar;
        this->toolBox->insertItem(0,bar,pageName);
    }
    pageMap[pageName]->addAction(action);
    return action;
}
