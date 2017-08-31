#include "graphicsselectscene.h"
#include <QDebug>

GraphicsSelectScene::GraphicsSelectScene(QObject *parent):
    QGraphicsScene(parent)
{
    this->view = NULL;
}

GraphicsSelectScene::~GraphicsSelectScene()
{

}

void GraphicsSelectScene::setGraphicsView(QGraphicsView * v)
{
    this->view = v;
}

void GraphicsSelectScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        cleanScene();
    }
    this->startx = event->scenePos().x();
    this->starty = event->scenePos().y();
    QGraphicsScene::mousePressEvent(event);
}

void GraphicsSelectScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    int sx = event->scenePos().x();
    int sy = event->scenePos().y();
    QTransform trsf;
    if(0 == this->itemAt(sx,sy,trsf))
    {
        cleanScene();
        int w = sx > startx ? sx - startx : startx - sx;
        int h = sy > starty ? sy - starty : starty - sy;
        QGraphicsRectItem * item = new QGraphicsRectItem(QRectF(0,0,w,h));
        item->setAcceptDrops(true);
        item->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
        this->addItem(item);
        item->setPos(startx,starty);
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void GraphicsSelectScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QTransform trsf;
    QGraphicsItem * item = this->itemAt(event->scenePos().x(),event->scenePos().y(),trsf);
    if(this->view != NULL && 0 == item)
    {
        cleanScene();
        this->view->setVisible(false);
    }
    else
    {
        QPoint p = view->mapFromScene(item->scenePos());
        QRectF r = item->boundingRect();
        emit rectSelected(view->mapToGlobal(p).x(),view->mapToGlobal(p).y(),r.height(),r.width());
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void GraphicsSelectScene::cleanScene()
{
    QList<QGraphicsItem *> items = this->items();
    while(!items.isEmpty())
    {
        this->removeItem(items.at(0));
        items.removeAt(0);
    }
}
