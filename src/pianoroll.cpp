#include "pianoroll.h"
#include"pianorollitem.h"
#include <QDebug>
#include <functional>
#include <math.h>
#include <QScrollBar>
#include <QList>
#include <src/midimanager.h>
#include <src/keyboard.h>
#include <src/velocityview.h>
QList<QGraphicsItem*> activeNotes;
int defaultCols = 50;
/*!
    \class PianoRoll
        \brief A widget which represents the graphical view of a piano roll.

        A piano roll is a two-dimensional representation of time vs. pitch.
        This class is a QGraphicsView which handles general user interation
        with the view. It is dependent on the PianoRollItem class which is
        the actual pitches and length, being stored in a QGraphicsScene.

        \note Neither this class nor its items are responsible for storing any
        information about the notes. Those are stored in the MidiManager class
        as an array.

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
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setMinimumWidth(1000);
    setMinimumHeight(200);

    scene = new QGraphicsScene;
    scene->setSceneRect(0,0,tPQN*cols,PianoRollItem::keyHeight*128);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    this->setScene(scene);

    colSpacing = width()/cols;
    sceneRect = new QRectF(0,0,tPQN*cols,PianoRollItem::keyHeight*128);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(ShowContextMenu(const QPoint &)));

    this->scale(((float)width() / (tPQN*cols)),1);
    viewport()->update();
    qDebug() << "Time: " << ((float)(60.0/120.0)*cols*1000);
    timer = new QTimeLine((float)(60.0/120.0)*cols*1000);//Song time in ms
    timer->setFrameRange(0,100);
    timer->setCurveShape(QTimeLine::LinearCurve);

    animation = new QGraphicsItemAnimation;
    QGraphicsRectItem *line = new QGraphicsRectItem(0,0,1,5000);
    scene->addItem(line);
    line->setZValue(1);
    QPen pen(Qt::red,0);
    line->setPen(pen);
    line->setBrush(Qt::red);
    line->setPos(0,0);
    animation->setItem(line);
    animation->setTimeLine(timer);
    rubberBand = new QRubberBand(QRubberBand::Rectangle, this);

    for (int i = 0; i < sceneRect->width(); ++i) {
        animation->setPosAt(i / (double)sceneRect->width(), QPointF(i, 0));
    }
}
/*Double clicking an empty part of the Pianoroll adds a note
 *Double clicking an existing note will remove it from the PianoRoll
 */
void PianoRoll::mouseDoubleClickEvent(QMouseEvent  *event)
{
    if(event->button()==Qt::LeftButton)
    {
        QGraphicsItem *item = itemAt(event->pos());

        if (item!=0) {
            //deletes an existing note from song
            PianoRollItem *pItem = static_cast<PianoRollItem*>(item);
            int note = 127 - (pItem->y()/PianoRollItem::keyHeight);

            velocityView->updateItems(pItem->noteStart,0,note,false);
            MidiManager::updateMidiDelete(pItem->noteStart,pItem->noteEnd,note,track->track);

            scene->removeItem(item);
            delete item;
            item=nullptr;
            activeNotes.clear();
        }
        else{
            //Adds a new note to song
            QPointF mousePos = mapToScene(event->pos());

            int length = tPQN*scaleFactor;
            int quadrant = (mousePos.x()/(tPQN * scaleFactor));

            quadrant *=scaleFactor*tPQN;;

            int newY = mousePos.y() / PianoRollItem::keyHeight;
            newY = newY*PianoRollItem::keyHeight;

            PianoRollItem *pNote = new PianoRollItem;
            scene->addItem(pNote);
            pNote->pianoroll = this;


            pNote->setPos(quadrant,newY);
            pNote->setBoundingRect(length);
            track->trackMidiView->updateViewItems(quadrant,length,newY);
            scene->update(0,0,tPQN*cols,PianoRollItem::keyHeight*128);
            pNote->noteStart = quadrant;
            pNote->noteEnd = length;
            int note =127-newY/PianoRollItem::keyHeight;
            velocityView->updateItems(quadrant,70,note,true);

            MidiManager::updateMidiAdd(note,70,quadrant,length,track->track);

            qDebug() << track->track->listOfNotes.length();
            QGraphicsView::mouseDoubleClickEvent(event);
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
        int note = 127 - (castNote->y()/PianoRollItem::keyHeight);
        playKeyboardNote(note,true);

        QTimer::singleShot(400,[=](){
            turnNoteOff(note);
        });
        activeNotes.append(castNote);
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
    rubberBand->setGeometry(QRect(origin,event->pos()).normalized());
    QGraphicsView::mouseMoveEvent(event);
}

void PianoRoll::mouseReleaseEvent(QMouseEvent *event)
{
    foreach (auto var, scene->selectedItems()) {
        activeNotes.append(var);
    }

    foreach (auto var, scene->selectedItems()) {
        PianoRollItem *castNote = dynamic_cast<PianoRollItem*>(var);
        castNote->brush = QBrush(Qt::darkCyan);
        castNote->update(castNote->boundingRect());
    }
    rubberBand->hide();
    QGraphicsView::mouseReleaseEvent(event);
}
/*! test */
void PianoRoll::clearActiveNotes()
{
    foreach (auto note, activeNotes) {
        PianoRollItem *castNote = dynamic_cast<PianoRollItem*>(note);
        if (castNote==0) {
            continue;
        }
        castNote->brush = QBrush(QColor(102, 179, 255));
        castNote->update(castNote->boundingRect());
    }
    activeNotes.clear();
}

//Takes midi data and converts to notes on a Pianoroll
void PianoRoll::convertTrackToItems()
{
    int dw =track->track->totalDT;
    totalDT = dw;
    //update the playback animation

    int tqn = dw/MidiManager::TPQN;
    if (tqn == 0) {
        tqn = cols;
        dw = MidiManager::TPQN*cols;
    }
    qDebug() << "totalDT:" << totalDT;
    scene->setSceneRect(0,0,dw,128*PianoRollItem::keyHeight);
    double qnotes = totalDT/(MidiManager::TPQN);
    timer->setDuration(((float)(60.0/120.0)*qnotes*1000));
    qDebug() <<"Time: " << timer->duration();
    animation->clear();
    for (int i = 0; i < dw; ++i) {
        animation->setPosAt(i / (double)dw, QPointF(i, 0));
    }
    //  velocityView->setSceneRect(0,0,dw,128*velocityView->height());
    int curNote = 0;
    int elapsedDW = 0;
    int noteEnd = 0;

    for(int i = 0; i < track->track->listOfNotes.length(); i+=3){
        elapsedDW += track->track->listOfNotes.at(i);
        //indicates a note. ignore other junk for now
        if((track->track->listOfNotes.at(i+2)& 0xF0) ==0x90){
            curNote = (track->track->listOfNotes.at(i+2) >> 8) & 127;
            //now I need to find its note off

            for(int j = i+5; j< track->track->listOfNotes.length(); j+=3){
                noteEnd+= track->track->listOfNotes.at(j -2);
                if(((track->track->listOfNotes.at(j) >> 8) & 127) == curNote ){

                    PianoRollItem *pNote = new PianoRollItem;
                    scene->addItem(pNote);
                    pNote->setPos(elapsedDW,128*PianoRollItem::keyHeight - curNote*PianoRollItem::keyHeight);
                    pNote->pianoroll = this;
                    pNote->setBoundingRect(noteEnd);
                    pNote->noteStart = elapsedDW;
                    pNote->noteEnd = noteEnd;
                    noteEnd = 0;
                    break;
                }
            }
        }
    }

    cols = tqn;
    tPQN = MidiManager::TPQN;
    colSpacing = (double)width()/cols;

    this->resetMatrix();
    scale((float)width() / (dw),1);
    while(tPQN*scaleFactor*transform().m11()<minimumColSpacing) {
        scaleFactor *=2;
    }
    this->viewport()->update();
    this->update();
    horizontalScrollBar()->setValue(0);
    verticalScrollBar()->setValue(0);


}

void PianoRoll::turnNoteOff(int note)
{
    playKeyboardNote(note,false);
}

void PianoRoll::updateSongTrackerPos(bool isPauseOrResume, bool isResume)
{
    if (isPauseOrResume) {
        if(isResume){
            timer->setPaused(false);
        }
        else{
            currentTimer = timer->currentTime();
            timer->setPaused(true);
        }

    }else{
        timer->setCurrentTime(0);
        timer->start();
    }

}

void PianoRoll::notifyPianoRollItemMoved(int xMove, int yMove, QGraphicsItem *item)
{
    foreach (auto note, activeNotes) {
        if(item == note)
            continue;
        note->setX(note->x() + xMove);
        note->setY(note->y() + yMove);
    }
}
/*!
  \fn PianoRoll::playKeyboardNote(int note, bool active)
  Given a piano note, either queue for the note to be played
  or to be turned off. Request is sent to active plugins for playback.
*/
void PianoRoll::playKeyboardNote(int note, bool active)
{
    track->plugin.host->eventToAdd.hasEventToAdd = true;
    track->plugin.host->eventToAdd.eventOn = active;
    track->plugin.host->eventToAdd.note = note;
}

void PianoRoll::deleteAllNotes()
{
    foreach (auto item, scene->items()) {
        scene->removeItem(item);
        delete item;
        item=nullptr;
    }
}

void PianoRoll::setScrollWheelValue(int value)
{
    QScrollBar* wheelPos;
    wheelPos=this->verticalScrollBar();
    wheelPos->setValue(value);
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

    connect(&action1,&QAction::triggered,this, [this]{scaleFactorChanged(8);});
    connect(&action2,&QAction::triggered,this, [this]{scaleFactorChanged(4);});
    connect(&action3,&QAction::triggered,this, [this]{scaleFactorChanged(2);});
    connect(&action4,&QAction::triggered,this, [this]{scaleFactorChanged(1);});
    connect(&action5,&QAction::triggered,this, [this]{scaleFactorChanged(0.5);});
    connect(&action6,&QAction::triggered,this, [this]{scaleFactorChanged(0.25);});


    contextMenu.addAction(&action1);
    contextMenu.addAction(&action2);
    contextMenu.addAction(&action3);
    contextMenu.addAction(&action4);
    contextMenu.addAction(&action5);
    contextMenu.addAction(&action6);


    contextMenu.exec(mapToGlobal(pos));
}
void PianoRoll::paintEvent(QPaintEvent *event)
{
    //Handles painting in background so as not to overlap notes
    QPainter *painter = new QPainter(this->viewport());
    drawBackground(painter,*sceneRect);
    QGraphicsView::paintEvent(event);
    delete painter;

}

void PianoRoll::drawBackground(QPainter * painter, const QRectF & rect)
{
    QPen pen;
    pen.setColor(Qt::lightGray);

    pen.setWidthF(0.5);
    painter->setPen(pen);
    painter->eraseRect(*sceneRect); //does this even matter?
    painter->resetMatrix();

    //Draws the horizontals lines

    for (int var = 0; var <= 128; ++var)
    {
        painter->drawLine(0,var*PianoRollItem::keyHeight-verticalScrollBar()->value(),
                          width()+horizontalScrollBar()->value(),var*PianoRollItem::keyHeight-verticalScrollBar()->value());
    }
    //Draws the vertical lines
    for (double var = 0; var < cols/scaleFactor; var+=1)
    {
        //Every whole note(if in scale) is drawn black
        if(fmod(var,4/scaleFactor) == 0)
        {
            pen.setColor(Qt::black);
            painter->setPen(pen);
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
    if (tPQN*scaleFactor*transform().m11()<minimumColSpacing) {
        scaleFactor = 32;
    }
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


        if (tPQN*scaleFactor*transform().m11()<minimumColSpacing) {
            scaleFactor = 16;
        }

        int zoom = event->angleDelta().y() /120;
        yscroller=event->angleDelta().y() /120*11;

        wheelPos = this->horizontalScrollBar();
        if (zoom > 0) {
            scale(xscale,1);
            velocityView->setScale(xscale,false,wheelPos->value());
        }
        else{
            scale(0.8/xscale,1);
            velocityView->setScale(0.8/xscale,false,wheelPos->value());
        }
        //  this->scale(xscale,1);
        if (transform().m11() < (float)width() / (tPQN*cols)) {
            resetMatrix();
            scale((float)width() / (tPQN*cols),1);
            velocityView->setScale((float)width() / (tPQN*cols),true,wheelPos->value());

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
    resetMatrix();
    float x = (float)width() / (tPQN*cols);
    scale(x,1);
    velocityView->onPianoRollResized(x);
    track->trackMidiView->onPianoRollResized(x);
}



