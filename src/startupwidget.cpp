#include "startupwidget.h"
#include <QDebug>

StartupWidget::StartupWidget(int c,QWidget *parent)
    : QWidget(parent),initCount(c)
{
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowMinimizeButtonHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    this->gView = new QGraphicsView(this);
    this->gView->setObjectName("gView");
    this->gView->setStyleSheet("QGraphicsView#gView{border-image:url(:/startup)}");
    this->gScene = new QGraphicsScene(this);

    this->gScene->setSceneRect(-200,-200,400,250);
    for(int i = 0;i < 10;i++)
    {
        i1[i].setPos(rand()%400 - 200,rand()%250 - 125);
        this->gScene->addItem(&i1[i]);
    }

    this->gView->setScene(this->gScene);
    this->resize(400,250);
    this->progressBar = new QProgressBar(this);
    this->progressBar->setRange(0,100);
    layout->setMargin(0);
    layout->addWidget(this->gView);
    layout->addWidget(this->progressBar);
    resize(400,250 + this->progressBar->height());
    timerid = startTimer(60);
}

StartupWidget::~StartupWidget()
{

}

void StartupWidget::timerEvent(QTimerEvent *event)
{
    timerid = event->timerId();
    QListIterator<QGraphicsLineItem *> iterator(lines);
    while(iterator.hasNext())
    {
        QGraphicsLineItem * item = iterator.next();
        this->gScene->removeItem(item);
        delete item;
    }
    lines.clear();

    for(int i = 0;i < 10;i++)
    {
        i1[i].moveToNext();
    }

    for(int i = 0;i < 10;i++)
    {
        QPainterPath pPath;
        pPath.addEllipse(i1[i].pos(),20,20);
        QList<QGraphicsItem *> items = gScene->items(pPath);
        QListIterator<QGraphicsItem *> itemIter(items);
        while(itemIter.hasNext())
        {
            QGraphicsItem * item = itemIter.next();
            if(item->type() != QGraphicsLineItem::Type)
            {
                QPen pen;
                pen.setColor(Qt::white);
                pen.setWidth(2);
                QGraphicsLineItem * lItem = gScene->addLine(item->x() - item->boundingRect().width()/2,
                                                            item->y() - item->boundingRect().height()/2,
                                                            i1[i].x() - i1[i].boundingRect().width()/2,
                                                            i1[i].y() - i1[i].boundingRect().height()/2,
                                                            pen);
                lines.append(lItem);
            }
        }
    }

}

void StartupWidget::initStateChange(QString state)
{
    //TODO state
    static int count = 0;
    count++;
    this->progressBar->setValue(count * 100 / initCount);
    if(count == initCount)
    {
        killTimer(timerid);
        this->close();
        emit finished();
    }
}
