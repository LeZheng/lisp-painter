#ifndef LSPLITCOPYWIDGET_H
#define LSPLITCOPYWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QEvent>
#include <QMouseEvent>

class LCloneableWidget:public QWidget
{
    Q_OBJECT
public:
    explicit LCloneableWidget(QWidget *parent = nullptr);
    virtual LCloneableWidget * clone() = 0;
protected:
    virtual bool eventFilter(QObject *watched, QEvent *event);
signals:
    void widgetActive(LCloneableWidget * w);
};

class LSplitCopyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LSplitCopyWidget(LCloneableWidget * w,QWidget *parent = nullptr);
    LCloneableWidget * getCurrentWidget();
private:
    bool isMouseLeftBtnPressed = false;
    QPoint oldPoint;
    LCloneableWidget * widget;
    LSplitCopyWidget * currentLeaf;
    void split(bool isH);
protected:
    virtual void resizeEvent(QResizeEvent *event);
signals:
    void widgetActive(LSplitCopyWidget * w);
public slots:
    bool closeWidget();
    void verticalSplit();
    void horizontalSplit();
private slots:
    void onCloneWidgetActive(LCloneableWidget * w);
    void onLeafWidgetActive(LSplitCopyWidget * w);
};

class LSplitLineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LSplitLineWidget(QWidget * parent = nullptr);
protected:
    virtual void mouseMoveEvent(QMouseEvent * e);
    virtual void mousePressEvent(QMouseEvent * e);
    virtual void mouseReleaseEvent(QMouseEvent * e);
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
private:
    bool isMouseLeftBtnPressed = false;
    QPoint lastPoint;
signals:
    void onDragged(int dx,int dy);
};

#endif // LSPLITCOPYWIDGET_H
