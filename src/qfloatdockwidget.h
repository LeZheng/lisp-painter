#ifndef QFLOATDOCKWIDGET_H
#define QFLOATDOCKWIDGET_H

#include <QWidget>
#include <QBitmap>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QApplication>

class QFloatDockWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QFloatDockWidget(QWidget *parent = 0);
protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void timerEvent(QTimerEvent *event);
private:
    QPoint mouseMovePos;
    bool isAppear;
    QRect originRect;
signals:

public slots:
    void appearOrDisappear(bool flag);
};

#endif // QFLOATDOCKWIDGET_H
