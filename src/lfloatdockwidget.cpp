#include "lfloatdockwidget.h"
#include <QDebug>

/*
 * This is LFloatDockWidget's constructor,\a l \a t \a w \a h means left,top,width and
 * height,respectively.This is the widget's max rect.
 */
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

/*
 * Overriding the QWidget's enterEvent method.The mouse enter this widget
 * when this widget's opacity is 0 will start the timer to resize and change
 * the opacity of this widget.Then the widget will appear.
 */
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

/*
 * Overriding the QWidget's leaveEvent method.The mouse enter this widget
 * when this widget's opacity is 1 will start the timer to resize and change
 * the opacity of this widget.Then the widget will disappear.
 */
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
        //If this widget is not overlapped,\fn appearOrDisappear won't be called.
        if(isOverlapped)
            appearOrDisappear(false);
    }
}

/*
 * This function will drop the widget using resize,move and setWindowOpacity.
*/
void LFloatDockWidget::dockDrop()
{
    QRect rect = this->frameGeometry();
    QRect dRect = QApplication::desktop()->availableGeometry();
    double opacity = isAppear ? (windowOpacity() + 0.05):(windowOpacity() - 0.05);

    if(dRect.top() >= rect.top() && dRect.bottom() > rect.bottom())//berth at top
    {
        int nextHeight = isAppear ? (rect.height() + originRect.height()/20 + 1):(rect.height() - originRect.height()/20 - 1);
        nextHeight = nextHeight >= 0 ? nextHeight : 2;
        if(nextHeight <= originRect.height())
        {
            resize(rect.width(),nextHeight);
        }
        this->setWindowOpacity(opacity);
    }
    else if(dRect.bottom() <= rect.bottom() && dRect.top() < rect.top())//berth at bottom
    {
        int nextHeight = isAppear ? (rect.height() + originRect.height()/20 + 1):(rect.height() - originRect.height()/20 - 1);
        nextHeight = nextHeight >= 0 ? nextHeight : 2;
        if(nextHeight <= originRect.height())
        {
            resize(rect.width(),nextHeight);
            move(rect.x(),dRect.bottom() - nextHeight + 1);
        }
        this->setWindowOpacity(opacity);
    }
    else if(dRect.left() >= rect.left() && dRect.right() != rect.right())//berth at left
    {
        int nextWidth = isAppear ? (rect.width() + originRect.width()/20 + 1):(rect.width() - originRect.width()/20 - 1);
        nextWidth = nextWidth >= 0 ? nextWidth : 2;
        if(nextWidth <= originRect.width())
        {
            resize(nextWidth,rect.height());
        }
        this->setWindowOpacity(opacity);
    }
    else if(dRect.left() != rect.left() && dRect.right() <= rect.right())//berth at right
    {
        int nextWidth = isAppear ? (rect.width() + originRect.width()/20 + 1):(rect.width() - originRect.width()/20 - 1);
        nextWidth = nextWidth >= 0 ? nextWidth : 2;
        if(nextWidth <= originRect.width())
        {
            resize(nextWidth,rect.height());
            move(dRect.right() - nextWidth + 1,rect.y());
        }
        this->setWindowOpacity(opacity);
    }

        if(windowOpacity() >= 1 || windowOpacity() <= 0)//drop end
        {
            if(opacity >= 1)
            {
                resize(originRect.width(),originRect.height());
                move(originRect.x(),originRect.y());
            }
            timer.stop();
            return;
        }
}

/*
 * Because of the Qt::FramelessWindowHint flag,overriding following three
 * method is necessarilly.
 */
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
    QRect nowRect = frameGeometry();
    QRect desktopRect = QApplication::desktop()->availableGeometry();
    if(nowRect.top() < desktopRect.top())
    {
        move(nowRect.left(),0);
    }
    else if(nowRect.left() < desktopRect.left())
    {
        move(desktopRect.left(),nowRect.top());
    }
    else if(nowRect.right() > desktopRect.right())
    {
        move(desktopRect.right() - nowRect.width() + 1,nowRect.top());
    }
    else if(nowRect.bottom() > desktopRect.bottom())
    {
        move(nowRect.left(),desktopRect.bottom() - nowRect.height() + 1);
    }
    originRect = frameGeometry();
    mouseMovePos = QPoint(0, 0);
}

/*
 * To start the timer.When the timer is active,this function is useless.
 */
void LFloatDockWidget::appearOrDisappear(bool flag)
{
    if(!this->timer.isActive() && (this->windowOpacity() >= 1 || windowOpacity() <= 0)){
        isAppear = flag;
        this->timer.start(10);
    }
}
