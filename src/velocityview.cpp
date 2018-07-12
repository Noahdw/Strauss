#include "velocityview.h"
#include <src/midimanager.h>
#include <QGraphicsRectItem>
#include <src/pianorollitem.h>
#include <QScrollBar>
#include <src/velocityviewitem.h>
#include <src/trackview.h>
int viewHeight = 70;
VelocityView::VelocityView(QWidget *parent) : QGraphicsView(parent)
{
    //  setSizePolicy(QSizePolicy ::Expanding , QSizePolicy ::Minimum );
    //setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setViewportUpdateMode(FullViewportUpdate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHint(QPainter::Antialiasing);
    //setContextMenuPolicy(Qt::CustomContextMenu);
    setMinimumWidth(1000);
    setMinimumHeight(70);
    setMaximumHeight(70);
    setMouseTracking(true);
    scene = new QGraphicsScene;
    scene->setSceneRect(0,0,MidiManager::TPQN*50,height());
    this->setScene(scene);
    this->scale(((float)width() / (MidiManager::TPQN*50)),1);

}

void VelocityView::addOrRemoveVelocityViewItem(int start, int velocity, int note, bool adding)
{
    int OldRange = (127 - 0);
    int NewRange = (height() - 0);
    int NewValue = (((velocity - 0) * NewRange) / OldRange) + 0;

    if (adding) {
        VelocityViewItem *line = new VelocityViewItem;
        scene->addItem(line);
        line->velocity = velocity;
        line->note = note;
        line->velocityView = this;
        line->viewHeight = height();
        line->setPos(start,height() - NewValue);
        //  qDebug() << "ADD" << start << "AND" << height() - NewValue;
        // resetTransform();
        // scale((float)width() / (MidiManager::TPQN*50),.2);
    }
    else
    {
        foreach (const auto& item, scene->items()) {
            if (floor(item->x()) == start) {
                 VelocityViewItem *line = dynamic_cast<VelocityViewItem*>(item);
                 if (line->note == note) {
                     scene->removeItem(line);
                     delete line;
                     return;
                 }
            }
        }
    }
}

void VelocityView::changeVelocityViewItemPosition(int oldPos, int newPos, int oldNote, int newNote)
{
    foreach (const auto& item, scene->items()) {
        if (item->x() == oldPos) {
             VelocityViewItem *line = dynamic_cast<VelocityViewItem*>(item);
             if (line->note == oldNote) {
                 line->note = newNote;
                 line->setX(newPos);

                 return;
             }
        }
    }
}

void VelocityView::populateVelocityViewFromTrack(TrackView *track)
{
    int vLength = track->track->listOfNotes.length();
    int DT = 0;
    for (int i = 0; i < vLength; i+=2) {
        DT += track->track->listOfNotes.at(i);
        if ((uchar)track->track->listOfNotes.at(i+1) == 0x90) {
            uchar velocity = (track->track->listOfNotes.at(i+1) >> 16);
            if (velocity > 0) {
                uchar note = (track->track->listOfNotes.at(i+1) >> 8);
                addOrRemoveVelocityViewItem(DT,velocity,note,true);
            }
        }
    }
}

void VelocityView::setScale(float x, bool needsReset, int wheelPos)
{
    if (needsReset) {
        resetMatrix();
    }
    this->scale(x,1);
    QScrollBar *wheel;
    wheel = this->horizontalScrollBar();
    wheel->setValue(wheelPos);

}

void VelocityView::paintEvent(QPaintEvent *event)
{
    QPainter painter(viewport());
    QPen pen;
    pen.setColor(Qt::lightGray);
    painter.setBrush(Qt::lightGray);

    painter.setPen(pen);
    painter.drawRect(viewport()->rect());
    QGraphicsView::paintEvent(event);
}


void VelocityView::onPianoRollResized(float x)
{
    resetMatrix();
    scale(x,1);
}
