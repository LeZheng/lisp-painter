#include "lsplitcopywidget.h"
#include <QDebug>

LCloneableWidget::LCloneableWidget(QWidget *parent):
    QWidget(parent){}

bool LCloneableWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::FocusIn || event->type() == QEvent::MouseButtonPress)
    {
        qDebug() << "widgetActive ..";
        emit widgetActive(this);
    }
    return QWidget::eventFilter(watched,event);
}

LSplitCopyWidget::LSplitCopyWidget(LCloneableWidget * w,QWidget *parent) : QWidget(parent)
{
    this->widget = w;
    QVBoxLayout * layout = new QVBoxLayout(this);
    widget->setParent(this);
    layout->addWidget(widget);
    layout->setContentsMargins(0,0,0,0);
}

void LSplitCopyWidget::verticalSplit()
{
    QLayout * l = layout();
    QVBoxLayout * mLayout = new QVBoxLayout(this);
    mLayout->setContentsMargins(0,0,0,0);
    LCloneableWidget * w1  = widget->clone();//TODO
    LSplitCopyWidget * scw1 = new LSplitCopyWidget(widget,this);
    LSplitCopyWidget * scw2 = new LSplitCopyWidget(w1,this);
    mLayout->addWidget(scw1);
    mLayout->addWidget(scw2);
    delete l;
    setLayout(mLayout);
}

void LSplitCopyWidget::horizontalSplit()
{
    QLayout * l = layout();
    QHBoxLayout * mLayout = new QHBoxLayout(this);
    mLayout->setContentsMargins(0,0,0,0);
    LCloneableWidget * w1  = widget->clone();//TODO
    LSplitCopyWidget * scw1 = new LSplitCopyWidget(widget,this);
    LSplitCopyWidget * scw2 = new LSplitCopyWidget(w1,this);
    mLayout->addWidget(scw1);
    mLayout->addWidget(scw2);
    delete l;
    setLayout(mLayout);
}
