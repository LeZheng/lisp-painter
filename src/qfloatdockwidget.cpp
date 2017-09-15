#include "qfloatdockwidget.h"
#include <QDebug>

QFloatDockWidget::QFloatDockWidget(QWidget *parent) : QWidget(parent)
{
    this->setMouseTracking(true);
    this->setWindowFlags(Qt::FramelessWindowHint);
    QPixmap mask(":/startup");
    setMask(QBitmap(mask.mask()));
    QPalette p;
    p.setBrush(QPalette::Window, QBrush(mask));
    setPalette(p);
    mouseMovePos = QPoint(0, 0);
    originRect = frameGeometry();
    connect(&timer,&QTimer::timeout,this,&QFloatDockWidget::dockDrop);
}

void QFloatDockWidget::enterEvent(QEvent *)
{
    QRect rect;
    rect = this->frameGeometry();
    QDesktopWidget *desktop = QApplication::desktop();
    if(rect.top()<= desktop->availableGeometry().top()||
            rect.bottom() >= desktop->availableGeometry().bottom() ||
            rect.left() <= desktop->availableGeometry().left() ||
            rect.right() >= desktop->availableGeometry().right())
    {
        appearOrDisappear(true);
    }
}

void QFloatDockWidget::leaveEvent(QEvent *)
{
    QRect rect;
    rect = this->frameGeometry();
    QDesktopWidget *desktop = QApplication::desktop();
    if(rect.top()<= desktop->availableGeometry().top()||
            rect.bottom() >= desktop->availableGeometry().bottom() ||
            rect.left() <= desktop->availableGeometry().left() ||
            rect.right() >= desktop->availableGeometry().right())
    {
        appearOrDisappear(false);
    }
}

void QFloatDockWidget::dockDrop()
{
    qreal opacity = isAppear ? windowOpacity() + 0.05:windowOpacity() - 0.05;
    this->setWindowOpacity(opacity);
    if(opacity >= 1 || opacity <= 0)
    {
        timer.stop();
        return;
    }

    QRect rect = this->frameGeometry();
    QRect dRect = QApplication::desktop()->availableGeometry();
    if(dRect.top() >= rect.top() && dRect.bottom() > rect.bottom())
    {
        int nextHeight = isAppear ? rect.height() + originRect.height()/20:rect.height() - originRect.height()/20;
        if(nextHeight <= originRect.height() && nextHeight >= 2)
        {
            resize(rect.width(),nextHeight);
        }
    }
    else if(dRect.bottom() <= rect.bottom() && dRect.top() < rect.top())
    {
        int nextHeight = isAppear ? rect.height() + originRect.height()/20:rect.height() - originRect.height()/20;
        if(nextHeight <= originRect.height() && nextHeight >= 2)
        {
            resize(rect.width(),nextHeight);
            move(rect.x(),dRect.bottom() - nextHeight + 1);
        }
    }
    else if(dRect.left() >= rect.left() && dRect.right() != rect.right())
    {
        int nextWidth = isAppear ? rect.width() + originRect.width()/20:rect.width() - originRect.width()/20;
        if(nextWidth <= originRect.width() && nextWidth >= 2)
        {
            resize(nextWidth,rect.height());
        }
    }
    else if(dRect.left() != rect.left() && dRect.right() <= rect.right())
    {
        int nextWidth = isAppear ? rect.width() + originRect.width()/20:rect.width() - originRect.width()/20;
        if(nextWidth <= originRect.width() && nextWidth >= 2)
        {
            resize(nextWidth,rect.height());
            move(dRect.right() - nextWidth + 1,rect.y());
        }
    }
}

void  QFloatDockWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(mouseMovePos != QPoint(0, 0))
    {
        move(geometry().x() + event->globalPos().x() - mouseMovePos.x(), geometry().y() + event->globalPos().y() - mouseMovePos.y());
        mouseMovePos = event->globalPos();
    }
}

void  QFloatDockWidget::mousePressEvent(QMouseEvent *event)
{
    mouseMovePos = event->globalPos();
}

void  QFloatDockWidget::mouseReleaseEvent(QMouseEvent *event)
{
    mouseMovePos = QPoint(0, 0);
}

void QFloatDockWidget::appearOrDisappear(bool flag)
{
    isAppear = flag;
    if(!this->timer.isActive())
        this->timer.start(10);
}
