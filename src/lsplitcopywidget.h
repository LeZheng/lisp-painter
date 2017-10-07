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
private:
    LCloneableWidget * widget;
signals:

public slots:
    void verticalSplit();
    void horizontalSplit();
};

#endif // LSPLITCOPYWIDGET_H
