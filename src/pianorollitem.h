#ifndef PIANOROLLNOTE_H
#define PIANOROLLNOTE_H

class VelocityView;


#include <QGraphicsItem>
#include <QPainter>
#include <QDrag>
#include <QGraphicsSceneMouseEvent>
#include <src/pianoroll.h>
#include <QScrollBar>
#include <QTimer>
class PianoRollItem : public QGraphicsItem
{

public:
    PianoRollItem();
    QBrush brush;
    static  const int keyHeight = 12;
    QRectF boundingRect() const;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    void setBoundingRect(int _width);
    int noteStart;
    int noteEnd;
    bool canMove = true;
    PianoRoll *pianoroll;
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
    int width;
    int lastXPos;
    int lastYPos;
    int initXPos;
    int initYPos;
    int lastYWithSound;

};


#endif // PIANOROLLNOTE_H
