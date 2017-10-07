#include "lfloatdockwidget.h"
#include <QDebug>

LFloatDockWidget::LFloatDockWidget(int l,int t,int w,int h,QWidget *parent) : QWidget(parent)
{
    this->setMouseTracking(true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    QPixmap mask(":/startup");
    setMask(QBitmap(mask.mask()));
    QPalette p;
    p.setBrush(QPalette::Window, QBrush(mask));
    setPalette(p);
    mouseMovePos = QPoint(0, 0);
    if(w != 0 && h != 0){
        move(l,t);
        resize(w,h);
    }
    originRect = frameGeometry();
    setMinimumHeight(5);
    setMinimumWidth(5);
    connect(&timer,&QTimer::timeout,this,&LFloatDockWidget::dockDrop);
    connect(this,&LFloatDockWidget::onDropDownOrUp,this,&LFloatDockWidget::appearOrDisappear);
}

LFloatDockWidget::~LFloatDockWidget()
{
    this->close();
}

void LFloatDockWidget::enterEvent(QEvent *)
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

void LFloatDockWidget::leaveEvent(QEvent *)
{
    QRect rect;
    rect = this->frameGeometry();
    QDesktopWidget *desktop = QApplication::desktop();
    if(rect.top()<= desktop->availableGeometry().top()||
            rect.bottom() >= desktop->availableGeometry().bottom() ||
            rect.left() <= desktop->availableGeometry().left() ||
            rect.right() >= desktop->availableGeometry().right())
    {
        if(isOverlapped)
            appearOrDisappear(false);
    }
}

void LFloatDockWidget::dockDrop()
{
    QRect rect = this->frameGeometry();
    QRect dRect = QApplication::desktop()->availableGeometry();
    qreal opacity = isAppear ? (windowOpacity() + 0.05):(windowOpacity() - 0.05);
    this->setWindowOpacity(opacity);
    if(opacity >= 1 || opacity <= 0)
    {
        if(opacity >= 1)
        {
            resize(originRect.width(),originRect.height());
            move(originRect.x(),originRect.y());
        }
        timer.stop();
        return;
    }

    if(dRect.top() >= rect.top() && dRect.bottom() > rect.bottom())
    {
        int nextHeight = isAppear ? (rect.height() + originRect.height()/20 + 1):(rect.height() - originRect.height()/20 - 1);
        if(nextHeight <= originRect.height())
        {
            resize(rect.width(),nextHeight);
        }
    }
    else if(dRect.bottom() <= rect.bottom() && dRect.top() < rect.top())
    {
        int nextHeight = isAppear ? (rect.height() + originRect.height()/20 + 1):(rect.height() - originRect.height()/20 - 1);
        if(nextHeight <= originRect.height())
        {
            resize(rect.width(),nextHeight);
            move(rect.x(),dRect.bottom() - nextHeight + 1);
        }
    }
    else if(dRect.left() >= rect.left() && dRect.right() != rect.right())
    {
        int nextWidth = isAppear ? (rect.width() + originRect.width()/20 + 1):(rect.width() - originRect.width()/20 - 1);
        if(nextWidth <= originRect.width())
        {
            resize(nextWidth,rect.height());
        }
    }
    else if(dRect.left() != rect.left() && dRect.right() <= rect.right())
    {
        int nextWidth = isAppear ? (rect.width() + originRect.width()/20 + 1):(rect.width() - originRect.width()/20 - 1);
        if(nextWidth <= originRect.width())
        {
            resize(nextWidth,rect.height());
            move(dRect.right() - nextWidth + 1,rect.y());
        }
    }
}

void  LFloatDockWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(mouseMovePos != QPoint(0, 0))
    {
        move(geometry().x() + event->globalPos().x() - mouseMovePos.x(), geometry().y() + event->globalPos().y() - mouseMovePos.y());
        mouseMovePos = event->globalPos();
    }
}

void  LFloatDockWidget::mousePressEvent(QMouseEvent *event)
{
    mouseMovePos = event->globalPos();
}

void  LFloatDockWidget::mouseReleaseEvent(QMouseEvent *event)
{
    mouseMovePos = QPoint(0, 0);
}

void LFloatDockWidget::appearOrDisappear(bool flag)
{
    if(!this->timer.isActive()){
        isAppear = flag;
        this->timer.start(10);
    }
}
