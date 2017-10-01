#ifndef QFLOATDOCKWIDGET_H
#define QFLOATDOCKWIDGET_H

#include <QWidget>
#include <QBitmap>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QApplication>
#include <QDockWidget>
#include <QTimer>

class QFloatDockWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QFloatDockWidget(int l = 0,int t = 0,int w = 0,int h = 0,QWidget *parent = 0);
    ~QFloatDockWidget();
    QRect originRect;
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

    QTimer timer;
public slots:
    void appearOrDisappear(bool flag);
private slots:
    void dockDrop();
};

#endif // QFLOATDOCKWIDGET_H
