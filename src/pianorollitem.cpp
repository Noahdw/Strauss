#include "pianorollitem.h"
#include <src/velocityview.h>
#include <QDebug>


PianoRollItem::PianoRollItem()
{
    brush = (QColor(102, 179, 255));
    setCacheMode(QGraphicsItem::NoCache);
    //   setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);
    setAcceptHoverEvents(true);
}

QRectF PianoRollItem::boundingRect() const
{
    return QRectF(0,0,width,keyHeight);
}
void PianoRollItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    QPen pen(Qt::black,0);
    painter->setPen(pen);
    painter->fillRect(boundingRect(),brush);
    painter->drawRect(boundingRect());
}

void PianoRollItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseDoubleClickEvent(event);
}

void PianoRollItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
    lastSceneResizePos = x();
    if (canResizeRight)
    {
        prepareGeometryChange();
        width += event->pos().x() - adjust;
        adjust = width;
        return;
    }
    else if (canResizeLeft)
    {

        setX(event->lastScenePos().x());
        prepareGeometryChange();
        qDebug() << "POS: " << event->pos().x() << " ADJUST: " << adjust << " Width: " << width;
        width = adjust - x();
        //adjust = (event->pos().x() - adjust) + (event->lastScenePos().x() - lastSceneResizePos);

        // lastSceneResizePos = event->lastScenePos().x();

        return;
    }

    int yPos = event->lastScenePos().y()/keyHeight;
    setY(yPos*keyHeight);


    int xMove = 0;
    int yMove = 0;
    double scaleFactor = pianoroll->tPQN * pianoroll->scaleFactor;

    int xPos = (x() / scaleFactor);
    int xAdjust = ( event->lastScenePos().x()/scaleFactor) - (actualInitX/ scaleFactor);
    if (pianoroll->prefferedScaleFactor <= 0.03125)
    {
        setPos(x() + event->lastScenePos().x() - actualInitX,yPos*keyHeight);
                xAdjust = event->lastScenePos().x() - actualInitX;
        actualInitX = event->lastScenePos().x();

    }
    else
    {
        if ((int)( event->lastScenePos().x()/scaleFactor) != (int)(actualInitX/ scaleFactor))
        {
            (int)( event->lastScenePos().x()/scaleFactor) < (int)(actualInitX/ scaleFactor) ? xAdjust = -1 : xAdjust = 1;
            actualInitX = event->lastScenePos().x();
        }
        // qDebug() << "scenePos: " << event->lastScenePos().x()/scaleFactor << "pos.x()" << actualInitX/ scaleFactor;
        xPos += xAdjust;
    }



    if (xPos < 0)
    {
        xPos = 0;
    }
    else if(x() + noteEnd > pianoroll->totalDT)
    {
        xPos = pianoroll->totalDT - noteEnd;
        setX(xPos);
        scaleFactor =1;
    }
    else if(pianoroll->prefferedScaleFactor > 0.03125)
    {
        setX(xPos*scaleFactor);
    }
    if(initXPos*scaleFactor !=  x())
    {
        xMove = x() - initXPos*scaleFactor;
        initXPos = x()/scaleFactor;
    }
    if(initYPos !=  y())
    {
        yMove = y() - initYPos;
        initYPos = y();
    }
    if (pianoroll->prefferedScaleFactor <= 0.03125)
    {
        xMove = xAdjust;
    }
    if(yMove != 0 || xMove != 0)
    {
        pianoroll->notifyPianoRollItemMoved(xMove,yMove,this); //update all other selected notes
    }
    //Perhaps a bad idea, but if a drag changes the yPos, play the new note
    if (lastYWithSound != yPos*keyHeight) {
        int note = 127 - yPos;
        pianoroll->playKeyboardNote(note,true);

        QTimer::singleShot(300,[=](){
            pianoroll->turnNoteOff(note);
        });
        lastYWithSound = yPos*keyHeight;
    }
}

void PianoRollItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    lastSceneResizePos = event->lastScenePos().x();
    initXPos = x();
    actualInitX = event->lastScenePos().x();
    initWidth = width;
    if (event->pos().x() <= (width / noteResizeThreshold) )
    {
        adjust = x() + width;
        canResizeLeft = true;
        return;
    }
    else if(event->pos().x() >= (width - width / noteResizeThreshold))
    {
        adjust = event->pos().x();
        canResizeRight = true;
        return;
    }
    int yPos = event->lastScenePos().y()/keyHeight;
    double scaleFactor =pianoroll->tPQN *pianoroll->scaleFactor;
    int xPos = event->lastScenePos().x()/scaleFactor;
    lastXPos = xPos*scaleFactor;
    lastYPos = yPos*keyHeight;
    initXPos = x()/scaleFactor;
    initYPos = lastYPos;
    lastYWithSound = lastYPos;
}

void PianoRollItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (canResizeLeft || canResizeRight)
    {
        if (initWidth != width || initXPos != x())
        {
            int velocity = MidiManager::getVelocityFromNote(noteStart,note,pianoroll->track->track);
            MidiManager::removeMidiNote(noteStart,noteEnd,note,pianoroll->track->track);
            noteStart = x();
            noteEnd = width;
            MidiManager::addMidiNote(note,velocity,x(),noteEnd,pianoroll->track->track);
            MidiManager::recalculateNoteListDT(pianoroll->track->track);
        }
        canResizeLeft = canResizeRight = false;
        return;
    }
    int yPos = event->lastScenePos().y()/keyHeight;
    double colSpacing =pianoroll->tPQN *pianoroll->scaleFactor;
    int xPos = event->lastScenePos().x()/colSpacing;

    if (((yPos*keyHeight) != lastYPos) || ((xPos*colSpacing) != lastXPos))
    {
        note = 127 - (lastYPos/keyHeight);
        int velocity = MidiManager::getVelocityFromNote(noteStart,note,pianoroll->track->track);
        MidiManager::removeMidiNote(noteStart,noteEnd,note,pianoroll->track->track);
        pianoroll->velocityView->changeVelocityViewItemPosition(noteStart,x(),note,127-yPos);
        //pianoroll->track->trackMidiView->deleteViewItem(noteStart,lastYPos);
        // pianoroll->track->trackMidiView->addViewItem(x(),width,yPos*keyHeight);
        note = 127 - yPos;
        MidiManager::addMidiNote(note,velocity,x(),noteEnd,pianoroll->track->track);
        noteStart = x();
        MidiManager::recalculateNoteListDT(pianoroll->track->track);
        pianoroll->changeNotesAfterMouseDrag(this);
    }
}

void PianoRollItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{

}

void PianoRollItem::setBoundingRect(int _width)
{
    prepareGeometryChange();
    width = _width;
}

void PianoRollItem::setInitalPosition(int start, int length, int note)
{
    setPos(start,(127 - note)*keyHeight);
    noteStart = start;
    noteEnd = length;
    this->note = note;
}
