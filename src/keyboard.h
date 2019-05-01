#ifndef KEYBOARD_H
#define KEYBOARD_H

class PianoRoll;
class PianoRollContainer;
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
    Keyboard(PianoRollContainer* p);
    PianoRoll *getPianoRoll(){return piano_roll;}
    PianoRollContainer* container(){return _container;}
    void updateWheel(QWheelEvent *event);
    void setNoteColors();
    int noteWidth = 24;
private:
    void addNotesToScene();
    QGraphicsScene *scene;
    PianoRoll *piano_roll;
    QGraphicsRectItem *activeNote;
    QBrush activeBrush;
    int lastNote;
    PianoRollContainer* _container;
protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent* event);
};

#endif // KEYBOARD_H
