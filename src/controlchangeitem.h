#ifndef CONTROLCHANGEITEM_H
#define CONTROLCHANGEITEM_H

class ControlChangeOverlay;

#include <QGraphicsItem>
#include <QWidget>
#include <QDebug>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QMap>

class ControlChangeItem : public QGraphicsItem
{
public:
    ControlChangeItem();
    QRectF boundingRect() const;
    QPainterPath shape() const;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void setInitalPos(QPointF pos);
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    int value = 0;
    int oldX;
    int oldY;
    ControlChangeOverlay * overlay;
private:
    const qreal radius = 6;
    int leftValue;
    int rightValue;
    bool firstEvent = true;
    QBrush brush;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

#endif // CONTROLCHANGEITEM_H
