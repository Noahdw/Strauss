#include "note.h"

Note::Note(float value) : noteValue(value)
{
    setCacheMode(QGraphicsItem::NoCache);
    setFlag(QGraphicsItem::ItemIgnoresTransformations,true);
    // setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);
}

QRectF Note::boundingRect() const
{
     return {-xRad/2,-xRad/2,xRad,xRad};
}

QPainterPath Note::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}

void Note::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();
    painter->setBrush(QBrush(Qt::black));
    painter->drawPath(shape());
}

void Note::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
   QGraphicsItem::mousePressEvent(event);
}
