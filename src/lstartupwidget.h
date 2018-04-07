#ifndef LSTARTUPWIDGET_H
#define LSTARTUPWIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "lgraphicsstaritem.h"
#include <QProgressBar>
#include <QVBoxLayout>
#include <QList>
#include <QTimerEvent>
#include <QSplashScreen>

class LStartupWidget : public QSplashScreen
{
    Q_OBJECT

public:
    LStartupWidget(int count,QWidget *parent = 0);
    ~LStartupWidget();
    void timerEvent(QTimerEvent *event);
    const int initCount;

    virtual void drawContents(QPainter *painter);

private:
    QGraphicsView * gView;
    QGraphicsScene * gScene;
    LGraphicsStarItem i1[10];
    QList<QGraphicsLineItem *> lines;
    QProgressBar * progressBar;
    QVBoxLayout * layout;
    int timerid;
signals:
    void finished();
public slots:
    void initStateChange(QString state);
};

#endif // LSTARTUPWIDGET_H
