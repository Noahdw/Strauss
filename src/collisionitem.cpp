#include "collisionitem.h"
#include "src/controlchangeoverlay.h"

CollisionItem::CollisionItem(ControlChangeOverlay *_parent)
    :parent(_parent)
{
    setCacheMode(QGraphicsItem::NoCache);
  //  setFlag(QGraphicsItem::ItemIgnoresTransformations,true);
   // setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);
}

QRectF CollisionItem::boundingRect() const
{
    return {0,0,width,3000};
}

QPainterPath CollisionItem::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void CollisionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(QBrush(Qt::black));
    painter->drawPath(shape());
}

void CollisionItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

     QGraphicsItem::mousePressEvent(event);
}

void CollisionItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //interpolate points and adjust width since mouseEvents skip some pixels
    auto pos = mapToScene(event->pos());
    prepareGeometryChange();
    width = pos.x() - x();
    auto list = collidingItems();
     qDebug() << list.size();
    parent->removeCollidingItems(list);
      setPos(pos.x(),0);
      prepareGeometryChange();
      width = 100;
    QGraphicsItem::mouseMoveEvent(event);
}

void CollisionItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

void CollisionItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseDoubleClickEvent(event);
}
