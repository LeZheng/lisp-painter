#ifndef GRAPHICSSELECTSCENE_H
#define GRAPHICSSELECTSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>

class GraphicsSelectScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsSelectScene(QObject *parent = 0);
    ~GraphicsSelectScene();
    void setGraphicsView(QGraphicsView * v);
signals:
    void rectSelected(int x,int y,int height,int width);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    int startx;
    int starty;
    QGraphicsView * view;
    void cleanScene();
};

#endif // GRAPHICSSELECTSCENE_H
