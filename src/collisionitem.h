#ifndef COLLISIONITEM_H
#define COLLISIONITEM_H

class ControlChangeOverlay;

#include <QGraphicsItem>
#include <QDebug>
#include <QPainter>
#include <QWidget>
#include <QGraphicsSceneMouseEvent>

class CollisionItem : public QGraphicsItem
{
public:
    CollisionItem(ControlChangeOverlay *_parent);
    QRectF boundingRect() const;
     QPainterPath shape() const;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    ControlChangeOverlay *parent;
    int lastX;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
private:
    qreal width = 500;
};

#endif // COLLISIONITEM_H
