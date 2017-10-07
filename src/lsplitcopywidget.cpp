#include "lsplitcopywidget.h"
#include <QDebug>

LCloneableWidget::LCloneableWidget(QWidget *parent):
    QWidget(parent){}

bool LCloneableWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::FocusIn || event->type() == QEvent::MouseButtonPress)
    {
        emit widgetActive(this);
    }
    return QWidget::eventFilter(watched,event);
}

LSplitCopyWidget::LSplitCopyWidget(LCloneableWidget * w,QWidget *parent) : QWidget(parent)
{
    currentLeaf = NULL;
    this->widget = w;
    QVBoxLayout * layout = new QVBoxLayout(this);
    widget->setParent(this);
    layout->addWidget(widget);
    layout->setContentsMargins(0,0,0,0);
    connect(widget,&LCloneableWidget::widgetActive,this,&LSplitCopyWidget::onCloneWidgetActive);
}

void LSplitCopyWidget::verticalSplit()
{
    if(widget != NULL)
    {
        disconnect(widget,&LCloneableWidget::widgetActive,this,&LSplitCopyWidget::onCloneWidgetActive);
        QLayout * l = layout();
        QVBoxLayout * mLayout = new QVBoxLayout(this);
        mLayout->setContentsMargins(0,0,0,0);
        LCloneableWidget * w1  = widget->clone();//TODO
        LSplitCopyWidget * scw1 = new LSplitCopyWidget(widget,this);
        LSplitCopyWidget * scw2 = new LSplitCopyWidget(w1,this);
        mLayout->addWidget(scw1);
        mLayout->addWidget(scw2);
        delete l;
        widget = NULL;
        setLayout(mLayout);
        this->currentLeaf = scw1;
        connect(scw1,&LSplitCopyWidget::widgetActive,this,&LSplitCopyWidget::onLeafWidgetActive);
        connect(scw2,&LSplitCopyWidget::widgetActive,this,&LSplitCopyWidget::onLeafWidgetActive);
    }
    else if(currentLeaf != NULL)
    {
        currentLeaf->verticalSplit();
    }
}

void LSplitCopyWidget::horizontalSplit()
{
    if(widget != NULL)
    {
        disconnect(widget,&LCloneableWidget::widgetActive,this,&LSplitCopyWidget::onCloneWidgetActive);
        QLayout * l = layout();
        QHBoxLayout * mLayout = new QHBoxLayout(this);
        mLayout->setContentsMargins(0,0,0,0);
        LCloneableWidget * w1  = widget->clone();//TODO
        LSplitCopyWidget * scw1 = new LSplitCopyWidget(widget,this);
        LSplitCopyWidget * scw2 = new LSplitCopyWidget(w1,this);
        mLayout->addWidget(scw1);
        mLayout->addWidget(scw2);
        delete l;
        widget = NULL;
        setLayout(mLayout);
        this->currentLeaf = scw1;
        connect(scw1,&LSplitCopyWidget::widgetActive,this,&LSplitCopyWidget::onLeafWidgetActive);
        connect(scw2,&LSplitCopyWidget::widgetActive,this,&LSplitCopyWidget::onLeafWidgetActive);
    }
    else if(currentLeaf != NULL)
    {
        currentLeaf->horizontalSplit();
    }
}

void LSplitCopyWidget::onCloneWidgetActive(LCloneableWidget * w)
{
    this->widget = w;
    emit widgetActive(this);
}

void LSplitCopyWidget::onLeafWidgetActive(LSplitCopyWidget * w)
{
    this->currentLeaf = w;
}
