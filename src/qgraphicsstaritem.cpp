#include "qgraphicsstaritem.h"
#include "QDebug"

QGraphicsStarItem::QGraphicsStarItem(QObject * parent):
    QObject(parent)
{
    img.load("/home/skyline/temp/weather-clear.png");//TODO
    vx = rand()%2 == 0 ? rand()%5 + 1 : -rand()%5 - 1;
    vy = rand()%2 == 0 ? rand()%5 + 1 : -rand()%5 - 1;
}

QRectF QGraphicsStarItem::boundingRect() const
{
    return QRectF(-img.width(),-img.height(),img.width(),img.height());
}

void QGraphicsStarItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF r = boundingRect();
    painter->drawPixmap(r.topLeft(),img);
}

void QGraphicsStarItem::moveToNext()
{
    QRectF sr = scene()->sceneRect();
    qreal edgeRight = sr.right();
    qreal edgeLeft = sr.left();
    qreal edgeTop = sr.top();
    qreal edgeBottom = sr.bottom();

    if(pos().x() > edgeRight)
        setPos(scene()->sceneRect().left(),pos().y());
    else if(pos().x() < edgeLeft)
        setPos(scene()->sceneRect().right(),pos().y());
    else if(pos().y() < edgeTop)
        setPos(pos().x(),scene()->sceneRect().bottom());
    else if(pos().y() > edgeBottom)
        setPos(pos().x(),scene()->sceneRect().top());
    else
        setPos(mapToParent(vx,vy));
}
