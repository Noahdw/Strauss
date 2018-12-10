#ifndef KEYBOARD_H
#define KEYBOARD_H

class PianoRoll;

#include <QWidget>
#include <QPainter>
#include <QGraphicsView>
#include <QMouseEvent>
#include <src/pianorollitem.h>
#include "QGraphicsRectItem"
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QScrollBar>
#include <QDebug>

class Keyboard: public QGraphicsView{
    Q_OBJECT
public:
    Keyboard(PianoRoll *pianoRoll,QWidget* parent = 0);
    PianoRoll *getPianoRoll(){return piano_roll;}
    void setScrollWheelValue(int value);
    void setNoteColors();
    int noteWidth = 24;
private:
    void addNotesToScene();
    QGraphicsScene *scene;
    PianoRoll *piano_roll;
    QGraphicsRectItem *activeNote;
    QBrush activeBrush;
    int lastNote;
protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
};

#endif // KEYBOARD_H
