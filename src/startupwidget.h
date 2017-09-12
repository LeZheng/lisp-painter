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
    StartupWidget(QWidget *parent = 0);
    ~StartupWidget();
    void timerEvent(QTimerEvent *event);
private:
    QGraphicsView * gView;
    QGraphicsScene * gScene;
    QGraphicsStarItem i1[10];
    QProgressBar * progressBar;
    QVBoxLayout * layout;
};

#endif // STARTUPWIDGET_H
