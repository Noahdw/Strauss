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
#include "src/trackmidi.h"

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
PianoRoll::PianoRoll(TrackMidi *trackMidi)
{
    midiTrack = trackMidi;
    setSizePolicy(QSizePolicy ::Expanding , QSizePolicy ::Expanding );
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setViewportUpdateMode(MinimalViewportUpdate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    // setContextMenuPolicy(Qt::CustomContextMenu);

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
    QGraphicsView::mouseDoubleClickEvent(event);
}
/*A click will select the item under the mouse
 * Clicking empty space will clear the selected items
 * A rightclick will open an option menu
 */

void PianoRoll::mousePressEvent(QMouseEvent *event)
{
    QGraphicsItem *currentItem = itemAt(event->pos());
    if(currentItem == NULL)
    {
        if(event->button()==Qt::LeftButton)
        {
            if (event->modifiers().testFlag(Qt::ControlModifier)) {

                last_selected_items = scene->selectedItems();
                origin = event->pos();
                setDragMode(QGraphicsView::RubberBandDrag);
                rubberBand->setGeometry(QRect(origin,QSize()));
                rubberBand->show();
                QGraphicsView::mousePressEvent(event);
                return;
            }

            //Adds a new note to song
            QPointF mousePos = mapToScene(event->pos());

            int length = tPQN * scaleFactor - 1; // -1 because reasons
            int quadrant = (mousePos.x()/(tPQN * scaleFactor));
            quadrant *=scaleFactor*tPQN;;
            int note = 128 - mousePos.y() / keyHeight;
            ItemData item = {quadrant,note,length};
            commands.push((Command*)new PianoRollAddCommand(this,{item}));
            commands.top()->execute();

            QGraphicsView::mousePressEvent(event);
            clearActiveNotes();
            return;
        }
    }


    if (event->button()==Qt::RightButton)
    {
        if(currentItem != NULL)
        {
            commands.push((Command*) new PianoRollRemoveCommand(this,{currentItem}));
            commands.top()->execute();
            clearActiveNotes();
        }
        else{
            ShowContextMenu(event->pos());
        }

        return;
    }
    QGraphicsItem *pNote = itemAt(event->pos());

    //If a note was clicked on
    if(pNote != NULL) {
        //Shift allows multiple selection of notes
        if (!event->modifiers().testFlag(Qt::ShiftModifier) && !last_selected_items.contains(pNote)) {
            clearActiveNotes();
            last_selected_items = scene->selectedItems();
            origin = event->pos();
            setDragMode(QGraphicsView::RubberBandDrag);
            rubberBand->setGeometry(QRect(origin,QSize()));
            rubberBand->show();

        }

        PianoRollItem *castNote = dynamic_cast<PianoRollItem*>(pNote);
        castNote->brush = QBrush(Qt::darkCyan);
        castNote->update(castNote->boundingRect());
        int note = 127 - (castNote->y()/keyHeight);
        playKeyboardNote(note,true);

        QTimer::singleShot(400,[=](){
            turnNoteOff(note);
        });
        if (!last_selected_items.contains(pNote))
        {
            pNote->setSelected(true);
            last_selected_items.push_back(pNote);
        }

    }
    //just the view/canvas clicked on
    else{
        if (!event->modifiers().testFlag(Qt::ShiftModifier)) {
            clearActiveNotes();
        }
        else
            last_selected_items = scene->selectedItems();
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
        if (!last_selected_items.contains(var)) {
            last_selected_items.push_back(var);
        }
    }
    rubberBand->hide();
    QGraphicsView::mouseReleaseEvent(event);
}

void PianoRoll::clearActiveNotes()
{
    foreach (auto note, last_selected_items) {
        PianoRollItem *castNote = dynamic_cast<PianoRollItem*>(note);
        if (castNote==0) {
            continue;
        }
        castNote->brush = QBrush(QColor(102, 179, 255));
        castNote->update(castNote->boundingRect());
    }
    scene->clearSelection();
    last_selected_items.clear();
}

//Takes track (midi) data and converts to notes on a Pianoroll
void PianoRoll::convertTrackToItems()
{
    int dw =midiTrack->midiData()->totalDT;
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

    for(int i = 0; i < midiTrack->midiData()->listOfNotes.length(); i+=2){
        elapsedDT += midiTrack->midiData()->listOfNotes.at(i);
        //indicates a note. ignore other junk for now
        int velocity = (uchar)(midiTrack->midiData()->listOfNotes.at(i+1) >> 16);
        if(velocity != 0){ // previously (midiTrack->midiData()->listOfNotes.at(i+1)& 0xF0) == 0x90
            curNote = (midiTrack->midiData()->listOfNotes.at(i+1) >> 8) & 127;
            //now I need to find its note off

            for(int j = i+2; j < midiTrack->midiData()->listOfNotes.length(); j+=2){
                noteEnd += midiTrack->midiData()->listOfNotes.at(j );
                if(((midiTrack->midiData()->listOfNotes.at(j+1) >> 8) & 127) == curNote ){

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
    // trackLengthView->fit
    //    trackLengthView->setScale(1,true,horizontalScrollBar()->value(),scaleFactor);
    //    trackLengthView->setTransform(transform());

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
    foreach (auto note, last_selected_items) {
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
    //midiTrack->midiData()MidiView->addViewItem(position,length,note*keyHeight);
    scene->update(0,0,tPQN*g_quarterNotes,keyHeight*128);
    velocityView->addOrRemoveVelocityViewItem(position,velocity,note,true);

}

void PianoRoll::changeNotesAfterMouseDrag(QGraphicsItem *item)
{
    for(const auto& note : last_selected_items)
    {
        if (item == note)
        {
            continue;
        }
        PianoRollItem *pNote = dynamic_cast<PianoRollItem*>(note);
        int velocity = MidiManager::getVelocityFromNote(pNote->noteStart,pNote->note,midiTrack->midiData());
        velocityView->addOrRemoveVelocityViewItem(pNote->noteStart,velocity,pNote->note,false);
        MidiManager::removeMidiNote(pNote->noteStart,pNote->noteEnd,pNote->note,midiTrack->midiData());
        pNote->noteStart = pNote->x();
        pNote->note = 127 - pNote->y()/keyHeight;
        MidiManager::addMidiNote(pNote->note,velocity,pNote->noteStart,pNote->noteEnd,midiTrack->midiData());
        velocityView->addOrRemoveVelocityViewItem(pNote->noteStart,velocity,pNote->note,true);
    }
    MidiManager::recalculateNoteListDT(midiTrack->midiData());
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

void PianoRoll::issueMoveCommand(int xMove, int yMove, QGraphicsItem *item)
{
    // changeNotesAfterMouseDrag(NULL);
    auto list = last_selected_items;
    list.detach();
    commands.append((Command*)new PianoRollMoveCommand(this,list,xMove,yMove,item));
    commands.top()->execute();
}

void PianoRoll::copyItems()
{
    copied_items = last_selected_items;
    copied_items.detach();
}

void PianoRoll::pasteItems()
{
//    for(const auto &item : copied_items)
//    {
//        auto *pitem = new PianoRollItem;

//    }
}

bool PianoRoll::hasPlugin()
{
    return  midiTrack->masterPlugin()->effect != NULL;
}
/*!
  \fn PianoRoll::playKeyboardNote(int note, bool active)
  Given a piano note, either queue for the note to be played
  or to be turned off. Request is sent to active plugins for playback.
*/
void PianoRoll::playKeyboardNote(int note, bool active)
{
    auto velocity =  (active ? 45 : 0);
    midiTrack->masterPlugin()->addMidiEvent(0x90,note,velocity,g_timer->currentValue());
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
    if (last_selected_items.size() == 0)
    {
        return;
    }
    auto list = last_selected_items;
    list.detach();
    commands.push((Command*) new PianoRollRemoveCommand(this,list));
    commands.top()->execute();
    clearActiveNotes();
    scene->selectedItems().clear();
    last_selected_items.clear();
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
   // painter->fillRect(viewport()->rect(),QBrush(QColor(250,250,250)));

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
// Called from the menu option
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
        int zoom  = event->angleDelta().y() /120;
        yscroller = event->angleDelta().y() /120*11;

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
    float x = (float)width() / (totalDT);
    scale(x,1);
    velocityView->onPianoRollResized(x);
    track->getTrackMidiView()->onPianoRollResized(x);
    horizontalScrollBar()->setValue(0);

    QGraphicsView::resizeEvent(event);
    if(tPQN*scaleFactor*transform().m11() > kminimumColSpacing)
    {
        scaleFactor = prefferedScaleFactor;
    }
    while(tPQN*scaleFactor*transform().m11()<kminimumColSpacing)
    {
        scaleFactor*=2;
    }
}

void PianoRoll::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier)
    {
        switch (event->key())
        {
        case Qt::Key_X:
            copyItems();
            deleteSelectedNotes(); // just delete for now
            break;
        case Qt::Key_Z:
            if (!commands.empty())
            {
                commands.pop()->undo();
            }
            break;
        case Qt::Key_C:
            copyItems();
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
        case Qt::Key_Up:
            issueMoveCommand(0,keyHeight * -1, NULL);
            return;
        case Qt::Key_Down:
            issueMoveCommand(0,keyHeight,NULL);
            return;
        case Qt::Key_Left:
        case Qt::Key_Right:
            return;
        default:
            break;
        }
    }
    QGraphicsView::keyPressEvent(event);
}





/*
I encapsulated many functions into classes so that I can undo actions on the piano roll.
*/

PianoRollMoveCommand::PianoRollMoveCommand(PianoRoll *pianoRoll, QList<QGraphicsItem*> items, int x, int y, QGraphicsItem *skipItem)
    :skippedItem(skipItem), items(items),xPos(x),yPos(y),_pianoRoll(pianoRoll)
{

}

void PianoRollMoveCommand::execute()
{
    foreach (auto note, items) {
        if(skippedItem == note)
            continue;
        note->setX(note->x() + xPos);
        note->setY(note->y() + yPos);
    }

    for(const auto& note : items)
    {
        if (skippedItem == note)
        {
            continue;
        }
        PianoRollItem *pNote = dynamic_cast<PianoRollItem*>(note);
        int velocity = MidiManager::getVelocityFromNote(pNote->noteStart,pNote->note,_pianoRoll->midiTrack->midiData());
        _pianoRoll->velocityView->addOrRemoveVelocityViewItem(pNote->noteStart,velocity,pNote->note,false);
        MidiManager::removeMidiNote(pNote->noteStart,pNote->noteEnd,pNote->note,_pianoRoll->midiTrack->midiData());
        pNote->noteStart = pNote->x();
        pNote->note = 127 - pNote->y()/keyHeight;
        MidiManager::addMidiNote(pNote->note,velocity,pNote->noteStart,pNote->noteEnd,_pianoRoll->midiTrack->midiData());
        _pianoRoll->velocityView->addOrRemoveVelocityViewItem(pNote->noteStart,velocity,pNote->note,true);
    }
    MidiManager::recalculateNoteListDT(_pianoRoll->midiTrack->midiData());
}


void PianoRollMoveCommand::undo()
{
    foreach (auto &note, items) {
        if(skippedItem == note)
            continue;
        note->setX(note->x() - xPos);
        note->setY(note->y() - yPos);
    }

    for(const auto& note : items)
    {
        if (skippedItem == note)
        {
            continue;
        }
        PianoRollItem *pNote = dynamic_cast<PianoRollItem*>(note);
        int velocity = MidiManager::getVelocityFromNote(pNote->noteStart,pNote->note,_pianoRoll->midiTrack->midiData());
        _pianoRoll->velocityView->addOrRemoveVelocityViewItem(pNote->noteStart,velocity,pNote->note,false);
        MidiManager::removeMidiNote(pNote->noteStart,pNote->noteEnd,pNote->note,_pianoRoll->midiTrack->midiData());
        pNote->noteStart = pNote->x();
        pNote->note = 127 - pNote->y()/keyHeight;
        MidiManager::addMidiNote(pNote->note,velocity,pNote->noteStart,pNote->noteEnd,_pianoRoll->midiTrack->midiData());
        _pianoRoll->velocityView->addOrRemoveVelocityViewItem(pNote->noteStart,velocity,pNote->note,true);
    }
    MidiManager::recalculateNoteListDT(_pianoRoll->midiTrack->midiData());
}


PianoRollAddCommand::PianoRollAddCommand(PianoRoll *pianoRoll, QList<ItemData> items)
    : items(items),_pianoRoll(pianoRoll)
{

}
void PianoRollAddCommand::execute()
{
    for(const auto& item : items)
    {
        PianoRollItem *pNote = new PianoRollItem;
        _pianoRoll->scene->addItem(pNote);
        pNote->pianoroll = _pianoRoll;
        pNote->setInitalPosition(item.xPos,item.length,item.yPos);
        pNote->setBoundingRect(item.length);
        _pianoRoll->velocityView->addOrRemoveVelocityViewItem(item.xPos,70,item.yPos,true);
        MidiManager::addMidiNote(item.yPos,70,item.xPos,item.length,_pianoRoll->midiTrack->midiData());
        newItems.append(pNote);
    }



    MidiManager::recalculateNoteListDT(_pianoRoll->midiTrack->midiData());
}


void PianoRollAddCommand::undo()
{
    foreach (const auto &item, newItems)
    {
        PianoRollItem *pItem = static_cast<PianoRollItem*>(item);
        int note = 127 - (pItem->y()/keyHeight);
        _pianoRoll->velocityView->addOrRemoveVelocityViewItem(pItem->noteStart,0,note,false);
        MidiManager::removeMidiNote(pItem->x(),pItem->width,note,_pianoRoll->midiTrack->midiData());
        _pianoRoll->scene->removeItem(item);
    }
    MidiManager::recalculateNoteListDT(_pianoRoll->midiTrack->midiData());
}

PianoRollAddCommand::~PianoRollAddCommand()
{

}


PianoRollRemoveCommand::PianoRollRemoveCommand(PianoRoll *pianoRoll, QList<QGraphicsItem *> items)
    : removedItems(items),_pianoRoll(pianoRoll)
{

}

void PianoRollRemoveCommand::execute()
{
    foreach (const auto& item, removedItems)
    {
        PianoRollItem *pItem = static_cast<PianoRollItem*>(item);
        int note = 127 - (pItem->y()/keyHeight);
        pItem->velocity = MidiManager::getVelocityFromNote(pItem->x(),note,_pianoRoll->midiTrack->midiData());
        _pianoRoll->velocityView->addOrRemoveVelocityViewItem(pItem->noteStart,pItem->velocity,note,false);
        MidiManager::removeMidiNote(pItem->x(),pItem->width,note,_pianoRoll->midiTrack->midiData());
        _pianoRoll->scene->removeItem(item);
    }
    MidiManager::recalculateNoteListDT(_pianoRoll->midiTrack->midiData());
}

void PianoRollRemoveCommand::undo()
{
    foreach (const auto& item, removedItems)
    {
        PianoRollItem *pItem = static_cast<PianoRollItem*>(item);

        int note = 127 - (pItem->y()/keyHeight);
        MidiManager::addMidiNote(note,pItem->velocity,pItem->x(),pItem->width,_pianoRoll->midiTrack->midiData());
        _pianoRoll->velocityView->addOrRemoveVelocityViewItem(pItem->x(),pItem->velocity,note,true);
        _pianoRoll->scene->addItem(item);
    }
    MidiManager::recalculateNoteListDT(_pianoRoll->midiTrack->midiData());
}
