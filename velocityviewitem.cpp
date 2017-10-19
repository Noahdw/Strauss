#include "velocityviewitem.h"

VelocityViewItem::VelocityViewItem()
{
    prepareGeometryChange();
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setFlag(QGraphicsItem::ItemIgnoresTransformations,true);
}

void VelocityViewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QRectF rect = boundingRect();
    //const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    QPen pen(Qt::black,0);
    painter->setPen(pen);
    QBrush brush;
    brush.setColor(Qt::black);
    painter->setClipRect(option->exposedRect);
   // painter->fillRect(rect,brush);
    painter->drawRect(rect);
}

QRectF VelocityViewItem::boundingRect() const
{
    return QRectF(0,0,1,velocity);
}

