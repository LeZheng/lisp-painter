#ifndef LGRAPHICSSTARITEM_H
#define LGRAPHICSSTARITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsScene>
#include <QDateTime>

class LGraphicsStarItem :public QObject, public QGraphicsItem
{
public:
    LGraphicsStarItem(QObject * parent = 0);
    QRectF boundingRect() const;
    void moveToNext();
    int vx;
    int vy;
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);
private:
    QPixmap img;
};

#endif // LGRAPHICSSTARITEM_H
