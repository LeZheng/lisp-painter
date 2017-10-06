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
