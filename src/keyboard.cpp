#include "keyboard.h"
#include <src/pianoroll.h>
#include "pianorollcontainer.h"
/*This class represents the keyboard to the left of the Piano Roll.
 *It emits MIDI data to the VST for playbck.
 * */

Keyboard::Keyboard(PianoRollContainer* p) : _container(p)
{
    piano_roll = p->pianoRoll();
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scene = new QGraphicsScene;
    scene->setSceneRect(0,0,noteWidth,keyHeight*128);
    this->setScene(scene);
    setMaximumWidth(noteWidth);
    addNotesToScene();
}

void Keyboard::updateWheel(QWheelEvent *event)
{
    if(event->modifiers().testFlag(Qt::ControlModifier))
        return;
    int yscroller= 0;
    yscroller=event->angleDelta().y() /120*14;
    verticalScrollBar()->setValue(verticalScrollBar()->value()-yscroller);
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
        if (var % 12 == 0)
        {
            QGraphicsTextItem *text = new QGraphicsTextItem();
            text->setTextInteractionFlags(Qt::NoTextInteraction);
            QString str = "C" + QString::number(10 - (var / 12));
            text->setPlainText(str);
            scene->addItem(text);
            text->setX(0);
            text->setY((var - 5)*keyHeight - 5);
        }
    }
}

void Keyboard::setNoteColors()
{
    int notepos = 1;
    for ( auto &item : scene->items())
    {
        QGraphicsRectItem *note = dynamic_cast<QGraphicsRectItem*>(item);
        if (!note) {
            continue;
        }
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
    }
}

void Keyboard::mousePressEvent(QMouseEvent *event)
{
    auto items = scene->items(mapToScene(event->pos()),Qt::ContainsItemShape);

    for(const auto & item : items)
    {
        QGraphicsRectItem *note = dynamic_cast<QGraphicsRectItem*>(item);
        if (note)
        {
            lastNote = item->y();
            activeBrush =note->brush();
            note->setBrush(Qt::red);
            activeNote = note;
            piano_roll->playKeyboardNote(127 - note->y()/keyHeight, true);
        }

    }


}

void Keyboard::mouseDoubleClickEvent(QMouseEvent *event)
{
    auto items = scene->items(mapToScene(event->pos()),Qt::ContainsItemShape);

    for(const auto & item : items)
    {
        QGraphicsRectItem *note = dynamic_cast<QGraphicsRectItem*>(item);
        if (note)
        {
            activeBrush =note->brush();
            note->setBrush(Qt::red);
            activeNote = note;
            piano_roll->playKeyboardNote(127 - note->y()/keyHeight, true);
        }

    }
}

void Keyboard::wheelEvent(QWheelEvent *event)
{
    int yscroller=event->angleDelta().y() /120*14;

    verticalScrollBar()->setValue(verticalScrollBar()->value()-yscroller);
    container()->pianoRoll()->verticalScrollBar()->setValue(verticalScrollBar()->value());
}


void Keyboard::mouseReleaseEvent(QMouseEvent *event)
{
    activeNote->setBrush(activeBrush);
    piano_roll->playKeyboardNote(127 - activeNote->y()/keyHeight, false);
}

void Keyboard::mouseMoveEvent(QMouseEvent *event)
{
    auto items = scene->items(mapToScene(event->pos()),Qt::ContainsItemShape);


    if (event->buttons() != Qt::LeftButton) {
        return;
    }
    for(const auto & item : items)
    {
        if (item->y() != lastNote) {
            if (!dynamic_cast<QGraphicsRectItem*>(item))
            {
                continue;
            }
            piano_roll->playKeyboardNote(127 - activeNote->y()/keyHeight, false);
            activeNote->setBrush(activeBrush);
            activeNote = dynamic_cast<QGraphicsRectItem*>(item);
            activeBrush = activeNote->brush();

            lastNote = item->y();
            activeNote->setBrush(Qt::red);
            piano_roll->playKeyboardNote(127 - activeNote->y()/keyHeight, true);

        }


    }
}

