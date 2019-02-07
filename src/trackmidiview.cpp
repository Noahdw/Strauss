#include "trackmidiview.h"
#include <src/trackview.h>
#include <src/pianorollitem.h>
/*
This class is a view of a midi track, it does not edit any midi data and is only for show.
Instances of this class belong to the TrackContainer which is located above the PianoRoll.
The TrackView class is composed with this one to represent a midi track for some given midi/plugin
*/
TrackMidiView::TrackMidiView(QWidget *parent)  : QGraphicsView(parent)
{
    setViewportUpdateMode(MinimalViewportUpdate);
    setMinimumHeight(115);
    setMaximumHeight(115);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    brush = QBrush(Qt::lightGray);
    pen   = QPen(Qt::black, 1, Qt::SolidLine);

}

void TrackMidiView::resizeEvent(QResizeEvent *event)
{
    fitInView(scene->sceneRect(),Qt::IgnoreAspectRatio);
}
void TrackMidiView::onPianoRollResized(float x)
{
    fitInView(scene->sceneRect(),Qt::IgnoreAspectRatio);
}

void TrackMidiView::shareScene(QGraphicsScene *scene)
{
    this->scene = scene;
    setScene(scene);
    totalDT = scene->width();
    fitInView(scene->sceneRect(),Qt::IgnoreAspectRatio);
}

void TrackMidiView::clickedOn(bool state)
{
    if (state)
    {
        pen.setStyle(Qt::DashLine);
        brush.setColor(QColor(170,170,170));
    }
    else
    {
        pen.setStyle(Qt::SolidLine);
        brush.setColor(Qt::lightGray);
    }
    viewport()->update();
}
