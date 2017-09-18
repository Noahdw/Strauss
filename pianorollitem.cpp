#include "pianorollitem.h"
#include <QDebug>


PianoRollItem::PianoRollItem()
{
    brush = (QColor(102, 179, 255));
    setCacheMode(QGraphicsItem::NoCache);
    setFlag(QGraphicsItem::ItemIsMovable, true);
}

QRectF PianoRollItem::boundingRect() const
{

    return QRectF(0,0,width,keyHeight);
}
void PianoRollItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    QRectF rect = boundingRect();
    //const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    QPen pen(Qt::black,0);
    painter->setPen(pen);

    painter->fillRect(rect,brush);
    painter->drawRect(rect);

}

void PianoRollItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

    QGraphicsItem::mouseDoubleClickEvent(event);

}

void PianoRollItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    int yPos = event->lastScenePos().y()/keyHeight;
    this->setY(yPos*keyHeight);


    double colSpacing =pianoroll->tPQN *pianoroll->scaleFactor;
    int xPos = event->lastScenePos().x()/colSpacing;
    qDebug() << xPos;
    this->setX(xPos*colSpacing);
}

void PianoRollItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    int yPos = event->lastScenePos().y()/keyHeight;
    double colSpacing =pianoroll->tPQN *pianoroll->scaleFactor;
    int xPos = event->lastScenePos().x()/colSpacing;
    lastXPos = xPos*colSpacing;
    lastYPos = yPos*keyHeight;
    qDebug() << "PRESSED";
}

void PianoRollItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    int yPos = event->lastScenePos().y()/keyHeight;
    double colSpacing =pianoroll->tPQN *pianoroll->scaleFactor;
    int xPos = event->lastScenePos().x()/colSpacing;

    if (((yPos*keyHeight) != lastYPos) || ((xPos*colSpacing) != lastXPos)) {
        qDebug() << "I did move";
    }
}

void PianoRollItem::setBoundingRect(int _width){

    prepareGeometryChange();
    this->width = _width;

}
