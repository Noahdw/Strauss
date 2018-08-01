#include "trackmidiview.h"
#include <src/trackview.h>
#include <src/pianorollitem.h>

TrackMidiView::TrackMidiView(QWidget *parent)  : QGraphicsView(parent)
{
    setViewportUpdateMode(MinimalViewportUpdate);
    setRenderHint(QPainter::Antialiasing);

    //  setMinimumWidth(400);
    setMinimumHeight(90);
    setMaximumHeight(90);
    //   setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    brush = QBrush(Qt::lightGray);
    pen   = QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

}


void TrackMidiView::paintEvent(QPaintEvent *event)
{
    QPainter painter(viewport());
    painter.setBrush(brush);
    painter.setPen(pen);
 //   painter.fillRect(viewport()->rect(),painter.brush());
    painter.drawRect(viewport()->rect());
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
