#include "keyboard.h"

Keyboard::Keyboard(QWidget *parent) : QGraphicsView(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
scene = new QGraphicsScene;
scene->setSceneRect(0,0,noteWidth,PianoRollItem::keyHeight*128);
this->setScene(scene);
setFixedSize(noteWidth,400);

addNotesToScene();
}



void Keyboard::addNotesToScene()
{
    int notepos = 1;
    for (int var = 0; var <= 128; ++var)
    {
        QGraphicsRectItem *note = new QGraphicsRectItem(0,0,noteWidth,PianoRollItem::keyHeight);
        switch (notepos) {
        case 1:
            note->setBrush(Qt::white);
            break;
        case 2:
            note->setBrush(Qt::black);
            break;
        case 3:
            note->setBrush(Qt::white);
            break;
        case 4:
            note->setBrush(Qt::white);
            break;
        case 5:
            note->setBrush(Qt::black);
            break;
        case 6:
            note->setBrush(Qt::white);
            break;
        case 7:
            note->setBrush(Qt::black);
            break;
        case 8:
            note->setBrush(Qt::white);
            break;
        case 9:
            note->setBrush(Qt::white);
            break;
        case 10:
            note->setBrush(Qt::black);
            break;
        case 11:
            note->setBrush(Qt::white);
            break;
        case 12:
            note->setBrush(Qt::black);
            break;
        default:
            break;
        }
        notepos++;


        QPen pen(Qt::black,0);
        note->setPen(pen);
        scene->addItem(note);
        note->setX(0);
        note->setY(var*PianoRollItem::keyHeight);
        if (notepos>12) {
            notepos = 1;
        }
    }
}

void Keyboard::scrollWheelChanged(int value)
{
     QScrollBar* wheelPos;
     wheelPos=this->verticalScrollBar();
     wheelPos->setValue(value);
}

void Keyboard::mousePressEvent(QMouseEvent *event)
{
    auto item = itemAt(event->pos());
     QGraphicsRectItem *note = dynamic_cast<QGraphicsRectItem*>(item);
     activeBrush =note->brush();
     note->setBrush(Qt::red);
    activeNote = note;
    //broken for now
    emit playSelectedNote(127 - note->y()/PianoRollItem::keyHeight, true);

}

void Keyboard::mouseDoubleClickEvent(QMouseEvent *event)
{
    auto item = itemAt(event->pos());
     QGraphicsRectItem *note = dynamic_cast<QGraphicsRectItem*>(item);
     activeBrush =note->brush();
     note->setBrush(Qt::red);
    activeNote = note;
//broken for now
   emit playSelectedNote(127 - note->y()/PianoRollItem::keyHeight, true);
}

void Keyboard::mouseReleaseEvent(QMouseEvent *event)
{
    activeNote->setBrush(activeBrush);
    //broken for now
   emit  playSelectedNote(127 - activeNote->y()/PianoRollItem::keyHeight, false);

}

