#ifndef PIANOROLLNOTE_H
#define PIANOROLLNOTE_H
#include <QGraphicsItem>
#include <QPainter>

class PianoRollItem : public QGraphicsItem
{

public:
    PianoRollItem();
    static  const int keyHeight = 12;
    QRectF boundingRect() const;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    void setBoundingRect(int _width);
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
private:
    int width;
};


#endif // PIANOROLLNOTE_H
