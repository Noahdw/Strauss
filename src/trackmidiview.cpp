#include "trackmidiview.h"
#include <src/trackview.h>
#include <src/pianorollitem.h>

TrackMidiView::TrackMidiView(QWidget *parent)  : QGraphicsView(parent)
{

    setViewportUpdateMode(MinimalViewportUpdate);
    setRenderHint(QPainter::Antialiasing);

    //  setMinimumWidth(400);
    setMinimumHeight(70);
    setMaximumHeight(90);
    //   setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}


void TrackMidiView::paintEvent(QPaintEvent *event)
{
    QPainter painter(viewport());
    QPen pen;
    pen.setColor(Qt::black);
    painter.setBrush(Qt::lightGray);
    painter.setPen(pen);
    painter.fillRect(viewport()->rect(),painter.brush());
    QGraphicsView::paintEvent(event);

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
