#ifndef NOTE_H
#define NOTE_H

#include <QObject>
#include <QPainter>
#include <QGraphicsItem>
#include "src/Notation/n_common.h"
class Note : public QGraphicsItem
{
public:
    Note(float value);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    float noteValue;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    const qreal xRad = n_noteWidth / 2.0;
    const qreal yRad = n_noteHeight / 2.0;
};

#endif // NOTE_H
