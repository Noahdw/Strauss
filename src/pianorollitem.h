#ifndef PIANOROLLNOTE_H
#define PIANOROLLNOTE_H

class VelocityView;


#include <QGraphicsItem>
#include <QPainter>
#include <QDrag>
#include <QGraphicsSceneMouseEvent>
#include <src/pianorollwidget.h>
#include <QScrollBar>
#include <QTimer>
#include <QGraphicsSceneHoverEvent>
class PianoRollItem : public QGraphicsItem
{

public:
    PianoRollItem();
    QRectF boundingRect() const;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    void setBoundingRect(int _width);
    void setInitalPosition(int start, int length, int note);
    PianoRollWidget *pianoroll;
    QBrush brush;

    int noteStart;
    int noteEnd;
    int note;
    bool canMove = true;
    int velocity;
    int width;
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);

private:

    int lastXPos;
    int lastYPos;
    int initXPos;
    int initYPos;
    int initWidth;
    int lastYWithSound;
    int initMouseXPos;

    const int noteResizeThreshold = 10; // in percent, so first x% and last x% allows resizing
    bool canResizeRight = false;
    bool canResizeLeft = false;
    int adjust;
    int lastSceneResizePos;
};

static  const double keyHeight = 12;
#endif // PIANOROLLNOTE_H
