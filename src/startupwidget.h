#ifndef STARTUPWIDGET_H
#define STARTUPWIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "qgraphicsstaritem.h"
#include <QProgressBar>
#include <QVBoxLayout>

class StartupWidget : public QWidget
{
    Q_OBJECT

public:
    StartupWidget(int count,QWidget *parent = 0);
    ~StartupWidget();
    void timerEvent(QTimerEvent *event);
    const int initCount;
private:
    QGraphicsView * gView;
    QGraphicsScene * gScene;
    QGraphicsStarItem i1[10];
    QProgressBar * progressBar;
    QVBoxLayout * layout;

public slots:
    void initStateChange(QString state);
};

#endif // STARTUPWIDGET_H
