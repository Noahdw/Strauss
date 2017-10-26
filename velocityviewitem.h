#ifndef VELOCITYVIEWITEM_H
#define VELOCITYVIEWITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

class VelocityViewItem  : public QGraphicsItem
{
public:
    VelocityViewItem();
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    void setBoundingRect(int _width);
     QRectF boundingRect() const;
    int note;
    int velocity;
};

#endif // VELOCITYVIEWITEM_H
