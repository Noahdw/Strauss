#ifndef VELOCITYVIEWITEM_H
#define VELOCITYVIEWITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsEllipseItem>
#include <src/velocityview.h>
class VelocityViewItem  : public QGraphicsItem
{
public:
    VelocityViewItem();
    void notifyVelocityChanged(int velocity);
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    void setBoundingRect(int _width);
    QRectF boundingRect() const;
    int note;
    int viewHeight;
    int velocity;
    QGraphicsEllipseItem *circle;
    QPainterPath shape() const;
    VelocityView * velocityView;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

#endif // VELOCITYVIEWITEM_H
