#include "keyboard.h"
#include <src/pianoroll.h>

/*This class represents the keyboard to the left of the Piano Roll.
 *It emits MIDI data to the VST for playbck.
 * */

Keyboard::Keyboard(QWidget *parent) : QGraphicsView(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scene = new QGraphicsScene;
    scene->setSceneRect(0,0,noteWidth,keyHeight*128);
    this->setScene(scene);
    // setFixedSize(noteWidth,400);
    setMaximumWidth(noteWidth);

    addNotesToScene();
}

void Keyboard::setPianoRoll(PianoRoll *proll)
{
    pianoroll = proll;
}

void Keyboard::setScrollWheelValue(int value)
{
    QScrollBar* wheelPos;
    wheelPos=this->verticalScrollBar();
    wheelPos->setValue(value);
}

void Keyboard::addNotesToScene()
{
    int notepos = 1;
    for (int var = 0; var <= 128; ++var)
    {
        QGraphicsRectItem *note = new QGraphicsRectItem(0,0,noteWidth,keyHeight);
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
        note->setY(var*keyHeight);
        if (notepos>12) {
            notepos = 1;
        }
    }
}

void Keyboard::mousePressEvent(QMouseEvent *event)
{
    auto item = itemAt(event->pos());
    QGraphicsRectItem *note = dynamic_cast<QGraphicsRectItem*>(item);
    activeBrush =note->brush();
    note->setBrush(Qt::red);
    activeNote = note;

    pianoroll->playKeyboardNote(127 - note->y()/keyHeight, true);

}

void Keyboard::mouseDoubleClickEvent(QMouseEvent *event)
{
    auto item = itemAt(event->pos());
    QGraphicsRectItem *note = dynamic_cast<QGraphicsRectItem*>(item);
    activeBrush =note->brush();
    note->setBrush(Qt::red);
    activeNote = note;
    pianoroll->playKeyboardNote(127 - note->y()/keyHeight, true);
}

void Keyboard::wheelEvent(QWheelEvent *event)
{
    int yscroller= 0;
    yscroller=event->angleDelta().y() /120*14;
    QScrollBar *wheelPos;
    wheelPos=this->verticalScrollBar();
    wheelPos->setValue(verticalScrollBar()->value()-yscroller);
    pianoroll->setScrollWheelValue(wheelPos->value());
}

void Keyboard::mouseReleaseEvent(QMouseEvent *event)
{
    activeNote->setBrush(activeBrush);
    pianoroll->playKeyboardNote(127 - activeNote->y()/keyHeight, false);
}

