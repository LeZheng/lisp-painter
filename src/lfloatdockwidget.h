/*
 * This widget will hide when near edge of desktop.
 * If you want to use it,you need add your own layout
 * or widget to it,like QWidget.
 */
#ifndef LFLOATDOCKWIDGET_H
#define LFLOATDOCKWIDGET_H

#include <QWidget>
#include <QBitmap>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QApplication>
#include <QDockWidget>
#include <QTimer>

class LFloatDockWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LFloatDockWidget(int l = 0,int t = 0,int w = 0,int h = 0,QWidget *parent = 0);
    ~LFloatDockWidget();
    QRect originRect;
    bool overlap(){return isOverlapped;}
    void setOverlap(bool flag){isOverlapped = flag;}
protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
signals:
    void onDropDownOrUp(bool flag);
private:
    QPoint mouseMovePos;
    bool isAppear;
    bool isOverlapped = false;
    QTimer timer;
public slots:
    void appearOrDisappear(bool flag);
private slots:
    void dockDrop();
};

#endif // LFLOATDOCKWIDGET_H
