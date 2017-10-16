#ifndef LSPLITCOPYWIDGET_H
#define LSPLITCOPYWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QEvent>

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
    LCloneableWidget * getWidget();
private:
    LCloneableWidget * widget;
    LSplitCopyWidget * currentLeaf;
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

#endif // LSPLITCOPYWIDGET_H
