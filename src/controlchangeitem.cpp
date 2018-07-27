#include "controlchangeitem.h"
#include "src/controlchangeoverlay.h"

ControlChangeItem::ControlChangeItem()
{
    setCacheMode(QGraphicsItem::NoCache);
    setFlag(QGraphicsItem::ItemIgnoresTransformations,true);
   // setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);
}

QRectF ControlChangeItem::boundingRect() const
{
    return {-radius/2,-radius/2,radius,radius};
}

QPainterPath ControlChangeItem::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}

void ControlChangeItem::setInitalPos(QPointF pos)
{
      setY(pos.y());
       setX(pos.x());
    if (pos.y() < 0)
    {
       setY(0);
    }
    else if(pos.y() > scene()->height())
    {
        setY(scene()->height());
    }
    else if(pos.x() < 0)
    {
        setX(0);
    }
    else if(pos.x() > scene()->width())
    {
        setX(scene()->width());
    }

}

void ControlChangeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();
    Qt::BrushStyle style = Qt::SolidPattern;
    QBrush brush(Qt::blue,style);
    painter->setBrush(brush);
    painter->drawPath(shape());
}
void ControlChangeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Get x-values left and right on the current item in the scene.
    // Both are used to ensure that the current item never moves beyond their x-values.
    oldX = x();
    oldY = y();
    auto i = overlay->activeItems.find(x());
    i--;
    leftValue = (*i).second->x();
    i++;
    i++;
    rightValue = (*i).second->x();
    QGraphicsItem::mousePressEvent(event);
}

void ControlChangeItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    if (leftValue > event->lastScenePos().x())
    {
       setX(leftValue + 1);
    }
    else if(rightValue < event->lastScenePos().x())
    {
        setX(rightValue - 1);
    }
    else
    {
    setX(event->lastScenePos().x());
    }

    if (event->lastScenePos().y() < 0)
    {
        setY(0);
    }else if(event->lastScenePos().y() > scene()->height() - radius)
    {
        setY(scene()->height()-radius);
    }
    else
    {
        setY(event->lastScenePos().y());

        int OldRange = (scene()->height() - 0);
        int NewRange = (127 - 1);
        int NewValue =127 - (((this->y() - 0) * NewRange) / OldRange);
        value = NewValue;
    }
    //  overlay->createLineConnector();

    overlay->createLineConnector();
    QGraphicsItem::mouseMoveEvent(event);
}

void ControlChangeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->lastScenePos().x() == oldX && event->lastScenePos().y() == oldY)
    {
        return;
    }
    if (event->lastScenePos().y() < 0)
    {
        setY(0);
    }else if(event->lastScenePos().y() > scene()->height() - radius)
    {
        setY(scene()->height()-radius*2);
    }

    int OldRange = (scene()->height() - 0);
    int NewRange = (127 - 1);
    int NewValue =127 - (((this->y() - 0) * NewRange) / OldRange);
    value = NewValue;
    overlay->viewport()->update();

    auto temp = this;
    overlay->activeItems.erase(oldX);
    overlay->activeItems[x()] = temp;
    overlay->createLineConnector();
    overlay->recalculateDT();
    QGraphicsItem::mouseReleaseEvent(event);
}

void ControlChangeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
QGraphicsItem::mouseDoubleClickEvent(event);
}

