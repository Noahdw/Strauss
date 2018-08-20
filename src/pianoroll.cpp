#include "pianoroll.h"
#include"pianorollitem.h"
#include <QDebug>
#include <functional>
#include <cmath>
#include <QScrollBar>
#include <QList>
#include <src/midimanager.h>
#include <src/keyboard.h>
#include <src/velocityview.h>


QList<QGraphicsItem*> activeNotes;

/*!
    \class PianoRoll
        \brief A widget which represents the graphical view of a piano roll.

        A piano roll is a two-dimensional representation of time vs. pitch.
        This class is a QGraphicsView which handles general user interation
        with the view. It is dependent on the PianoRollItem class which is
        the actual pitches and length, being stored in a QGraphicsScene.

        \note Neither this class nor its items are responsible for storing any
        information about the notes. Those are stored in the MidiManager class
        a both a map and vector.

*/

// ./qdoc C:/Users/Puter/Documents/MidiInter/midiinter.qdocconf
// Use this from a cmd line within the dir of qdoc.exe to create an html
//./qhelpgenerator C:/Users/Puter/Documents/MidiInter/doc/midiinter.qph -o midiinter.qch
//  use this to create a qch from our qph
PianoRoll::PianoRoll(QWidget *parent) : QGraphicsView(parent)
{
    setSizePolicy(QSizePolicy ::Expanding , QSizePolicy ::Expanding );
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setViewportUpdateMode(MinimalViewportUpdate);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setContextMenuPolicy(Qt::CustomContextMenu);

    scene = new QGraphicsScene;
    scene->setSceneRect(0,0,tPQN*g_quarterNotes,keyHeight*128);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);
    sceneRect = new QRectF(0,0,tPQN*g_quarterNotes,keyHeight*128);
    colSpacing = width()/g_quarterNotes;
    fitInView(scene->sceneRect());

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(ShowContextMenu(const QPoint &)));


    rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    animation  = new QGraphicsItemAnimation;
    line       = new QGraphicsRectItem(0,0,1,5000);

    scene->addItem(line);
    line->setZValue(1);
    QPen pen(QColor(156,21,25),0);
    line->setPen(pen);
    line->setBrush(QColor(156,21,25));
    line->setPos(0,-50);

    animation->setItem(line);
    animation->setTimeLine(g_timer);
    animation->setPosAt(0.0, QPointF(0, 0));
    animation->setPosAt(1.0, QPointF(scene->width(), 0));

}

PianoRoll::~PianoRoll()
{
    this->parentWidget()->deleteLater();
}
/*Double clicking an empty part of the Pianoroll adds a note.
 *Double clicking an existing note will remove it from the PianoRoll.
 */
void PianoRoll::mouseDoubleClickEvent(QMouseEvent  *event)
{
    if(event->button()==Qt::LeftButton)
    {
        QGraphicsItem *item = itemAt(event->pos());

        if (item!=0) {
            //deletes an existing note from song
            PianoRollItem *pItem = static_cast<PianoRollItem*>(item);
            int note = 127 - (pItem->y()/keyHeight);

            velocityView->addOrRemoveVelocityViewItem(pItem->noteStart,0,note,false);
            MidiManager::removeMidiNote(pItem->noteStart,pItem->noteEnd,note,track->track);
            scene->removeItem(item);
            delete item;
            item=nullptr;
            clearActiveNotes();

        }
        else{
            //Adds a new note to song
            QPointF mousePos = mapToScene(event->pos());

            int length = tPQN*scaleFactor - 1;
            int quadrant = (mousePos.x()/(tPQN * scaleFactor));
            quadrant *=scaleFactor*tPQN;;
            int note = 128 - mousePos.y() / keyHeight;

            PianoRollItem *pNote = new PianoRollItem;
            scene->addItem(pNote);
            pNote->pianoroll = this;
            pNote->setInitalPosition(quadrant,length,note);
            pNote->setBoundingRect(length);
            //   track->trackMidiView->addViewItem(quadrant,length,newY);
            scene->update(0,0,tPQN*g_quarterNotes,keyHeight*128);
            velocityView->addOrRemoveVelocityViewItem(quadrant,70,note,true);

            MidiManager::addMidiNote(note,70,quadrant,length,track->track);
            MidiManager::recalculateNoteListDT(track->track);
            QGraphicsView::mouseDoubleClickEvent(event);
            clearActiveNotes();
        }
    }
}
/*A click will select the item under the mouse
 * Clicking empty space will clear the selected items
 * A rightclick will open an option menu
 */

void PianoRoll::mousePressEvent(QMouseEvent *event)
{

    if (event->button()==Qt::RightButton)
    {
        ShowContextMenu(event->pos());
        return;
    }
    QGraphicsItem *pNote = itemAt(event->pos());

    //If a note was clicked on
    if(pNote!=0) {
        //Shift allows multiple selection of notes
        if (!event->modifiers().testFlag(Qt::ShiftModifier)) {
            clearActiveNotes();

        }

        PianoRollItem *castNote = dynamic_cast<PianoRollItem*>(pNote);
        castNote->brush = QBrush(Qt::darkCyan);
        castNote->update(castNote->boundingRect());
        int note = 127 - (castNote->y()/keyHeight);
        playKeyboardNote(note,true);

        QTimer::singleShot(400,[=](){
            turnNoteOff(note);
        });
        if (!scene->selectedItems().contains(pNote))
        {
            pNote->setSelected(true);
        }

    }
    //just the view/canvas clicked on
    else{
        if (!event->modifiers().testFlag(Qt::ShiftModifier)) {
            clearActiveNotes();
        }
        origin = event->pos();
        setDragMode(QGraphicsView::RubberBandDrag);
        rubberBand->setGeometry(QRect(origin,QSize()));
        rubberBand->show();
    }
    QGraphicsView::mousePressEvent(event);
}

void PianoRoll::mouseMoveEvent(QMouseEvent *event)
{
    // rubberBand->setGeometry(QRect(origin,event->pos()).normalized());
    QGraphicsView::mouseMoveEvent(event);
}

void PianoRoll::mouseReleaseEvent(QMouseEvent *event)
{

    foreach (auto var, scene->selectedItems()) {
        PianoRollItem *castNote = dynamic_cast<PianoRollItem*>(var);
        castNote->brush = QBrush(Qt::darkCyan);
        castNote->update(castNote->boundingRect());
    }
    rubberBand->hide();
    QGraphicsView::mouseReleaseEvent(event);
}

void PianoRoll::clearActiveNotes()
{
    foreach (auto note, scene->selectedItems()) {
        PianoRollItem *castNote = dynamic_cast<PianoRollItem*>(note);
        if (castNote==0) {
            continue;
        }
        castNote->brush = QBrush(QColor(102, 179, 255));
        castNote->update(castNote->boundingRect());
    }
    scene->clearSelection();
}

//Takes track (midi) data and converts to notes on a Pianoroll
void PianoRoll::convertTrackToItems()
{
    int dw =track->track->totalDT;
    totalDT = dw;
    //update the playback animation

    int tqn = dw/MidiManager::TPQN;
    if (tqn == 0) {
        tqn = g_quarterNotes;
    }
    scene->setSceneRect(0,0,dw,128*keyHeight);
    animation->clear();

    animation->setPosAt(0.0, QPointF(0, 0));
    animation->setPosAt(1.0, QPointF(dw, 0));
    int curNote = 0;
    int elapsedDT = 0;
    int noteEnd = 0;

    for(int i = 0; i < track->track->listOfNotes.length(); i+=2){
        elapsedDT += track->track->listOfNotes.at(i);
        //indicates a note. ignore other junk for now
        int velocity = (uchar)(track->track->listOfNotes.at(i+1) >> 16);
        if(velocity != 0){ // previously (track->track->listOfNotes.at(i+1)& 0xF0) == 0x90
            curNote = (track->track->listOfNotes.at(i+1) >> 8) & 127;
            //now I need to find its note off

            for(int j = i+2; j < track->track->listOfNotes.length(); j+=2){
                noteEnd += track->track->listOfNotes.at(j );
                if(((track->track->listOfNotes.at(j+1) >> 8) & 127) == curNote ){

                    PianoRollItem *pNote = new PianoRollItem;
                    scene->addItem(pNote);
                    pNote->setInitalPosition(elapsedDT,noteEnd,curNote);
                    pNote->setBoundingRect(noteEnd);
                    pNote->pianoroll = this;
                    pNote->setBoundingRect(noteEnd);
                    pNote->noteStart = elapsedDT;
                    pNote->noteEnd = noteEnd;
                    noteEnd = 0;
                    break;
                }
            }
        }
    }

    tPQN = MidiManager::TPQN;
    colSpacing = (double)width()/g_quarterNotes;

    resetMatrix();
    float x = (float)width() / (tPQN*g_quarterNotes);
    scale(x,1);
    while(tPQN*scaleFactor*transform().m11() < kminimumColSpacing)
    {
        scaleFactor *=2;
    }
    horizontalScrollBar()->setValue(0);
    verticalScrollBar()->setValue(0);
}

void PianoRoll::turnNoteOff(int note)
{
    playKeyboardNote(note,false);
}
// -1 indicates that no custom playback was provided. Should remove whole pause/resume
//thing and just make it dependent on custom
void PianoRoll::updateSongTrackerPos(bool isPauseOrResume, bool isResume, int custom)
{
    // qDebug() << "Timer req: " << custom;
    if (custom != -1)
    {
        double percent = custom/(double)totalDT;
        percent *= (double)g_timer->duration();
        g_timer->setPaused(true);
        g_timer->setCurrentTime(percent);
        g_timer->setPaused(false);
    }
    else{
        if (isPauseOrResume) {
            if(isResume){
                g_timer->setPaused(false);
            }
            else{
                currentTimer = g_timer->currentTime();
                g_timer->setPaused(true);
            }

        }else{
            g_timer->setCurrentTime(0);
            g_timer->start();
        }
    }
}
// Called when a group of items is moved
void PianoRoll::notifyPianoRollItemMoved(int xMove, int yMove, QGraphicsItem *item)
{
    foreach (auto note, scene->selectedItems()) {
        if(item == note)
            continue;
        note->setX(note->x() + xMove);
        note->setY(note->y() + yMove);
    }
}

void PianoRoll::addNoteToScene(int note, int position, int length, int velocity)
{
    PianoRollItem *pNote = new PianoRollItem;
    scene->addItem(pNote);
    pNote->pianoroll = this;
    pNote->setInitalPosition(position,length,note);
    pNote->setBoundingRect(length);
    //track->trackMidiView->addViewItem(position,length,note*keyHeight);
    scene->update(0,0,tPQN*g_quarterNotes,keyHeight*128);
    velocityView->addOrRemoveVelocityViewItem(position,velocity,note,true);
}

void PianoRoll::changeNotesAfterMouseDrag(QGraphicsItem *item)
{
    for(const auto& note : scene->selectedItems())
    {
        if (item == note)
        {
            continue;
        }
        PianoRollItem *pNote = dynamic_cast<PianoRollItem*>(note);
        int velocity = MidiManager::getVelocityFromNote(pNote->noteStart,pNote->note,track->track);
        velocityView->addOrRemoveVelocityViewItem(pNote->noteStart,velocity,pNote->note,false);
        MidiManager::removeMidiNote(pNote->noteStart,pNote->noteEnd,pNote->note,track->track);
        pNote->noteStart = pNote->x();
        pNote->note = 127 - pNote->y()/keyHeight;
        MidiManager::addMidiNote(pNote->note,velocity,pNote->noteStart,pNote->noteEnd,track->track);
        velocityView->addOrRemoveVelocityViewItem(pNote->noteStart,velocity,pNote->note,true);
    }
    MidiManager::recalculateNoteListDT(track->track);
}

void PianoRoll::switchViewContainer()
{

}

void PianoRoll::forceResize()
{
    resetMatrix();
    float x = (float)width() / (tPQN*g_quarterNotes);
    scale(x,1);
    velocityView->onPianoRollResized(x);
    track->getTrackMidiView()->onPianoRollResized(x);
    horizontalScrollBar()->setValue(0);

}
/*!
  \fn PianoRoll::playKeyboardNote(int note, bool active)
  Given a piano note, either queue for the note to be played
  or to be turned off. Request is sent to active plugins for playback.
*/
void PianoRoll::playKeyboardNote(int note, bool active)
{
    auto velocity =  (active ? 70 : 0);
    track->plugin.host->addMidiEvent(0x90,note,velocity,g_timer->currentValue());
   // track->plugin.host->eventToAdd.hasEventToAdd = true;
   // track->plugin.host->eventToAdd.eventOn = active;
  //  track->plugin.host->eventToAdd.note = note;
}

void PianoRoll::deleteAllNotes()
{
    foreach (auto item, scene->items()) {
        scene->removeItem(item);
        delete item;
        item=nullptr;
    }
}

void PianoRoll::deleteSelectedNotes()
{
    for(const auto& var : scene->selectedItems())
    {
        PianoRollItem *item = dynamic_cast<PianoRollItem*>(var);
        scene->removeItem(var);
        int note = 127 - item->y()/keyHeight;
        int velocity = MidiManager::getVelocityFromNote(item->x(),note,track->track);
        MidiManager::removeMidiNote(item->x(),item->noteEnd,note,track->track);
        velocityView->addOrRemoveVelocityViewItem(item->x(),velocity,note,false);
        delete item;
        item=nullptr;
    }
    scene->selectedItems().clear();
}

void PianoRoll::setScrollWheelValue(int value)
{
    verticalScrollBar()->setValue(value);
}

void PianoRoll::setKeyboard(Keyboard *kboard)
{
    keyboard = kboard;
}

Keyboard *PianoRoll::getKeyboard()
{
    return keyboard;
}

void PianoRoll::setVelocityView(VelocityView *view)
{
    velocityView = view;
}

void PianoRoll::ShowContextMenu(const QPoint &pos)
{
    QMenu contextMenu(("Context menu"), this);

    QAction action1("2 bars", this);
    QAction action2("1 bars", this);
    QAction action3("1/2 bars", this);
    QAction action4("1/4 bars", this);
    QAction action5("1/8 bars", this);
    QAction action6("1/16 bars", this);
    QAction action7("1/32 bars", this);
    QAction action8("Off", this);

    connect(&action1,&QAction::triggered,this, [this]{scaleFactorChanged(8);});
    connect(&action2,&QAction::triggered,this, [this]{scaleFactorChanged(4);});
    connect(&action3,&QAction::triggered,this, [this]{scaleFactorChanged(2);});
    connect(&action4,&QAction::triggered,this, [this]{scaleFactorChanged(1);});
    connect(&action5,&QAction::triggered,this, [this]{scaleFactorChanged(0.5);});
    connect(&action6,&QAction::triggered,this, [this]{scaleFactorChanged(0.25);});
    connect(&action7,&QAction::triggered,this, [this]{scaleFactorChanged(0.125);});
    connect(&action8,&QAction::triggered,this, [this]{scaleFactorChanged(0.03125);});

    contextMenu.addAction(&action1);
    contextMenu.addAction(&action2);
    contextMenu.addAction(&action3);
    contextMenu.addAction(&action4);
    contextMenu.addAction(&action5);
    contextMenu.addAction(&action6);
    contextMenu.addAction(&action7);
    contextMenu.addAction(&action8);
    contextMenu.exec(mapToGlobal(pos));
}
void PianoRoll::paintEvent(QPaintEvent *event)
{
    //Handles painting in background so as not to overlap notes
    QPainter painter(viewport());
    drawBackground(&painter,*sceneRect);
    QGraphicsView::paintEvent(event);
}

void PianoRoll::drawBackground(QPainter * painter, const QRectF & rect)
{
    QPen pen;
    pen.setColor(Qt::lightGray);
    pen.setWidthF(0.5);
    painter->setPen(pen);
    painter->eraseRect(*sceneRect); //does this even matter?
    painter->resetMatrix();
    painter->fillRect(viewport()->rect(),QBrush(QColor(250,250,250)));
    //Draws the horizontals lines

    for (int var = 0; var <= 128; ++var)
    {
        painter->drawLine(0,var*keyHeight-verticalScrollBar()->value(),
                          width()+horizontalScrollBar()->value(),var*keyHeight-verticalScrollBar()->value());
    }
    //Draws the vertical lines
    for (double var = 0; var < g_quarterNotes/scaleFactor; var+=1)
    {
        //Every whole note(if in scale) is drawn black
        if(fmod(var,4/scaleFactor) == 0)
        {
            pen.setColor(Qt::black);
            painter->setPen(pen);
        }
        if (prefferedScaleFactor <= 0.03125 )
        {
            continue;
        }
        painter->drawLine(var*tPQN*scaleFactor*transform().m11()-horizontalScrollBar()->value(),0,
                          var*tPQN*scaleFactor*transform().m11()-horizontalScrollBar()->value(),verticalScrollBar()->value()+height());

        pen.setColor(Qt::lightGray);
        painter->setPen(pen);
    }
}


void PianoRoll::scaleFactorChanged(double scale)
{
    scaleFactor = scale;
    prefferedScaleFactor = scale;
    while(tPQN*scaleFactor*transform().m11()<kminimumColSpacing)
    {
        scaleFactor*=2;
    }
    trackLengthView->setScale(1,false,horizontalScrollBar()->value(),scaleFactor);
    this->viewport()->update();
}

void PianoRoll::wheelEvent(QWheelEvent *event)
{

    int yscroller = 0;
    QScrollBar* wheelPos;
    if(event->modifiers().testFlag(Qt::ControlModifier))
    {
        //AnchorUnderMouse allows xPos to update automatically but make sure to keep track
        // of yPos as it should not be changed here.
        int previousMousePos = verticalScrollBar()->value();

        if(tPQN*scaleFactor*transform().m11() > kminimumColSpacing)
        {
            scaleFactor = prefferedScaleFactor;
        }
        while(tPQN*scaleFactor*transform().m11()<kminimumColSpacing)
        {
            scaleFactor*=2;
        }
        int zoom = event->angleDelta().y() /120;
        yscroller=event->angleDelta().y() /120*11;

        wheelPos = this->horizontalScrollBar();
        if (zoom > 0)
        {
            scale(xscale,1);
            velocityView->setScale(xscale,false,wheelPos->value());
            trackLengthView->setScale(xscale,false,wheelPos->value(),scaleFactor);
            trackLengthView->setTransform(transform());
        }
        else
        {
            scale(0.8/xscale,1);
            velocityView->setScale(0.8/xscale,false,wheelPos->value());
            trackLengthView->setScale(0.8/xscale,false,wheelPos->value(),scaleFactor);
            trackLengthView->setTransform(transform());
        }
        //  this->scale(xscale,1);
        if (transform().m11() <= (float)width() / (tPQN*g_quarterNotes))
        {
            resetMatrix();
            scale((float)width() / (tPQN*g_quarterNotes),1);
            velocityView->setScale((float)width() / (tPQN*g_quarterNotes),true,wheelPos->value());
            trackLengthView->setTransform(transform());
            trackLengthView->setScale((float)width() / (tPQN*g_quarterNotes),true,0,scaleFactor);
        }
        colSpacing*=transform().m11();
        wheelPos=this->verticalScrollBar();
        wheelPos->setValue(previousMousePos);
    }
    else
    {
        yscroller=event->angleDelta().y() /120*14;
        wheelPos=this->verticalScrollBar();
        wheelPos->setValue(verticalScrollBar()->value()-yscroller);
        keyboard->setScrollWheelValue(wheelPos->value());
    }

}

void PianoRoll::resizeEvent(QResizeEvent *event)
{
    if (!isInitialized)
    {
        return;
    }
    resetMatrix();
    float x = (float)width() / (tPQN*g_quarterNotes);
    scale(x,1);
    velocityView->onPianoRollResized(x);
    track->getTrackMidiView()->onPianoRollResized(x);
    horizontalScrollBar()->setValue(0);
    QGraphicsView::resizeEvent(event);
}

void PianoRoll::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier)
    {
        switch (event->key())
        {
        case Qt::Key_X:
            deleteSelectedNotes(); // just delete for now
            break;
        default:
            break;
        }
    }
    else
    {
        switch (event->key())
        {
        case Qt::Key_Delete:
            deleteSelectedNotes();
            break;
        default:
            break;
        }
    }
    QGraphicsView::keyPressEvent(event);
}



