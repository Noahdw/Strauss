#include "velocityviewitem.h"
#include <QDebug>
VelocityViewItem::VelocityViewItem()
{
   // prepareGeometryChange();
    setCacheMode(QGraphicsItem::NoCache);
    setFlag(QGraphicsItem::ItemIgnoresTransformations,true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);

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

void VelocityViewItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
     qDebug() << "WEF" << endl;
}

void VelocityViewItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    setY(event->lastScenePos().y());
    qDebug() << "fdfd" << endl;
}

void VelocityViewItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

}

void VelocityViewItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseDoubleClickEvent(event);
}

