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
    void setInitalPos(QPointF pos);
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    int value;
    int oldX;
    int oldY;
    ControlChangeOverlay * overlay;
private:
    const qreal radius = 6;
    int leftValue;
    int rightValue;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

#endif // CONTROLCHANGEITEM_H
