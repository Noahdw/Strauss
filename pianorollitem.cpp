#include "pianorollitem.h"
#include <QDebug>

PianoRollItem::PianoRollItem()
{

}



QRectF PianoRollItem::boundingRect() const
{

          return QRectF(0,0,width,keyHeight);
}
void PianoRollItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    QRectF rect = boundingRect();
    QBrush brush(QColor(102, 179, 255));
    QPen pen(Qt::black,1);
    painter->setPen(pen);
    painter->fillRect(rect,brush);
    painter->drawRect(rect);

}

void PianoRollItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

    QGraphicsItem::mouseDoubleClickEvent(event);

}

void PianoRollItem::setBoundingRect(int _width){

      prepareGeometryChange();
      this->width = _width;
      update();
}
