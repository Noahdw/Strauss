#include "trackmidiview.h"
#include <src/trackview.h>
#include <src/pianorollitem.h>

TrackMidiView::TrackMidiView(QWidget *parent)  : QGraphicsView(parent)
{
    setViewportUpdateMode(MinimalViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setMinimumHeight(100);
    setMaximumHeight(100);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    brush = QBrush(Qt::lightGray);
    pen   = QPen(Qt::black, 1, Qt::SolidLine);

}


void TrackMidiView::paintEvent(QPaintEvent *event)
{
    QPainter painter(viewport());
    painter.setBrush(brush);
    painter.setPen(pen);
    painter.drawRect(0,0,width() - 3, height() - 3);
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
