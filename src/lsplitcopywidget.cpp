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
    this->setMouseTracking(true);
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
    activateWindow();
    if(widget != NULL)
    {
        disconnect(widget,&LCloneableWidget::widgetActive,this,&LSplitCopyWidget::onCloneWidgetActive);
        QLayout * l = layout();
        QBoxLayout * mLayout;
        if(isHorizontal)
            mLayout = new QHBoxLayout();
        else
            mLayout = new QVBoxLayout();
        mLayout->setContentsMargins(0,0,0,0);
        LCloneableWidget * w1  = widget->clone();
        LSplitCopyWidget * scw1 = new LSplitCopyWidget(widget,this);
        LSplitCopyWidget * scw2 = new LSplitCopyWidget(w1,this);
        LSplitLineWidget * lineWidget = new LSplitLineWidget(this);
        mLayout->addWidget(scw1,isHorizontal ? (width() - 5) / 2 : (height() - 5) / 2);//TODO
        mLayout->addWidget(lineWidget,5);
        mLayout->addWidget(scw2,isHorizontal ? (width() - 5) / 2 : (height() - 5) / 2);//TODO
        delete l;
        widget = NULL;
        setLayout(mLayout);
        connect(scw1,&LSplitCopyWidget::widgetActive,this,&LSplitCopyWidget::onLeafWidgetActive);
        connect(scw2,&LSplitCopyWidget::widgetActive,this,&LSplitCopyWidget::onLeafWidgetActive);
        connect(lineWidget,&LSplitLineWidget::onDragged,
                [=](int dx,int dy)
        {
            if(isHorizontal)
            {
                mLayout->setStretch(0,mLayout->stretch(0) + dx);
                mLayout->setStretch(2,mLayout->stretch(2) - dx);
            }
            else
            {
                mLayout->setStretch(0,mLayout->stretch(0) + dy);
                mLayout->setStretch(2,mLayout->stretch(2) - dy);
            }
        });
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
            if(w != this->currentLeaf && w != NULL && w->inherits("LSplitCopyWidget"))
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
            else
            {
                delete w;
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

void LSplitCopyWidget::resizeEvent(QResizeEvent *event)
{
    QSize size = event->size();
    QLayout * l = layout();
    if(l->inherits("QBoxLayout"))
    {
        QBoxLayout * layout = qobject_cast<QBoxLayout *>(l);
        if(layout->count() == 3)
        {
            layout->setStretch(1,5);
            layout->setStretch(0,layout->inherits("QVBoxLayout") ? (size.height() - 5) / 2 : (size.width() - 5) / 2);
            layout->setStretch(2,layout->inherits("QVBoxLayout") ? (size.height() - 5) / 2 : (size.width() - 5) / 2);
        }
    }
    else
        qWarning() << "layout is not inherits QBoxLayout";
}

LSplitLineWidget::LSplitLineWidget(QWidget * parent):QWidget(parent)
{
    setMinimumHeight(3);
    setMinimumWidth(3);
}

void LSplitLineWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(isMouseLeftBtnPressed)
    {
        int dx = e->globalX() - lastPoint.x(),dy = e->globalY() - lastPoint.y();
        if(dx != 0 || dy != 0)
            emit onDragged(dx,dy);
        lastPoint = QPoint(e->globalX(),e->globalY());
    }
}

void LSplitLineWidget::mousePressEvent(QMouseEvent *e)
{
    isMouseLeftBtnPressed = true;
    lastPoint = QPoint(e->globalX(),e->globalY());
    QWidget::mousePressEvent(e);
}

void LSplitLineWidget::mouseReleaseEvent(QMouseEvent *e)
{
    isMouseLeftBtnPressed = false;
    lastPoint = QPoint(0,0);
    QWidget::mouseReleaseEvent(e);
}

void LSplitLineWidget::leaveEvent(QEvent *event)
{
    setCursor(Qt::ArrowCursor);
    QWidget::leaveEvent(event);
}

void LSplitLineWidget::enterEvent(QEvent *event)
{
    setCursor(width() > height() ? Qt::SizeVerCursor : Qt::SizeHorCursor);
    QWidget::enterEvent(event);
}
