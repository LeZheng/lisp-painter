#include "ltoolswidget.h"
#include "ui_ltoolswidget.h"

LToolsWidget::LToolsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LToolsWidget)
{
    ui->setupUi(this);
    this->tabWidget = this->ui->tabWidget;
    this->tabWidget->setAutoFillBackground(true);
    QPalette myPalette;
    myPalette.setBrush(backgroundRole(),Qt::black);
    this->tabWidget->setPalette(myPalette);
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
        this->tabWidget->addTab(toolbar,pageName);
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
        this->tabWidget->addTab(page,pageName);
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
        this->tabWidget->addTab(bar,pageName);
    }
    pageMap[pageName]->addAction(action);
    cleaner.add(action);
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
        this->tabWidget->addTab(bar,pageName);
    }
    pageMap[pageName]->addAction(action);
    return action;
}
