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
    split(false);
}

void LSplitCopyWidget::horizontalSplit()
{
    split(true);
}

void LSplitCopyWidget::split(bool isHorizontal)
{
    if(widget != NULL)
    {
        disconnect(widget,&LCloneableWidget::widgetActive,this,&LSplitCopyWidget::onCloneWidgetActive);
        QLayout * l = layout();
        QLayout * mLayout;
        if(isHorizontal)
            mLayout = new QHBoxLayout();
        else
            mLayout = new QVBoxLayout();
        mLayout->setContentsMargins(0,0,0,0);
        LCloneableWidget * w1  = widget->clone();
        LSplitCopyWidget * scw1 = new LSplitCopyWidget(widget,this);
        LSplitCopyWidget * scw2 = new LSplitCopyWidget(w1,this);
        mLayout->addWidget(scw1);
        mLayout->addWidget(scw2);
        delete l;
        widget = NULL;
        setLayout(mLayout);
        connect(scw1,&LSplitCopyWidget::widgetActive,this,&LSplitCopyWidget::onLeafWidgetActive);
        connect(scw2,&LSplitCopyWidget::widgetActive,this,&LSplitCopyWidget::onLeafWidgetActive);
        emit scw2->widgetActive(scw2);
    }
    else if(currentLeaf != NULL)
    {
        currentLeaf->split(isHorizontal);
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
    emit widgetActive(this);
}

bool LSplitCopyWidget::closeWidget()
{
    if(this->widget != NULL)
    {
        delete this->widget;
        this->widget = NULL;
        return false;
    }
    else if(this->currentLeaf != NULL && !this->currentLeaf->closeWidget())
    {
        QLayout * oldLayout = layout();
        QVBoxLayout * layout = new QVBoxLayout();
        QLayoutItem *child;
        while((child = oldLayout->takeAt(0)) != 0)
        {
            QWidget * w = child->widget();
            if(w != this->currentLeaf && w != NULL)
            {
                LSplitCopyWidget * splitW = qobject_cast<LSplitCopyWidget*>(w);
                this->widget = splitW->getCurrentWidget();
                widget->setParent(this);
                layout->addWidget(widget);
                layout->setContentsMargins(0,0,0,0);
                connect(widget,&LCloneableWidget::widgetActive,this,&LSplitCopyWidget::onCloneWidgetActive);
            }
            else if(w == this->currentLeaf && w != NULL)
            {
                delete w;
                this->currentLeaf = NULL;
            }
            delete child;
        }
        delete oldLayout;
        setLayout(layout);
        return true;
    }
    return false;
}

LCloneableWidget * LSplitCopyWidget::getCurrentWidget()
{
    if(this->widget == NULL)
        this->currentLeaf->getCurrentWidget();
    else
        return this->widget;
}
