#ifndef QGRAPHICSSTARITEM_H
#define QGRAPHICSSTARITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsScene>
#include <QDateTime>

class QGraphicsStarItem :public QObject, public QGraphicsItem
{
public:
    QGraphicsStarItem(QObject * parent = 0);
    QRectF boundingRect() const;
    void moveToNext();
    int vx;
    int vy;
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);
private:
    QPixmap img;
};

#endif // QGRAPHICSSTARITEM_H
