#include "tracklengthview.h"
#include "src/midimanager.h"
/*
This class represents a multi-use bar above the piano roll. It displays
the current time of the track that is dependent on how zoomed in you
are in the piano roll. It also allows for a user to start playback from
a specific spot as well as to zoom the piano roll by dragging up/down.
*/
TrackLengthView::TrackLengthView(QWidget *parent) : QGraphicsView(parent)
{
    setViewportUpdateMode(MinimalViewportUpdate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setMinimumWidth(1000);
    setMinimumHeight(20);
    scene = new QGraphicsScene;
    scene->setSceneRect(0,0,MidiManager::TPQN*50,height());
    this->setScene(scene);
    this->scale(((float)width() / (MidiManager::TPQN*50)),1);
}

void TrackLengthView::paintEvent(QPaintEvent *event)
{
    QPainter *painter = new QPainter(this->viewport());
    QPen pen;
    pen.setColor(Qt::lightGray);
    painter->setBrush(Qt::lightGray);
    painter->setPen(pen);
    painter->drawRect(viewport()->rect());
}

void TrackLengthView::mouseDoubleClickEvent(QMouseEvent *event)
{

}

void TrackLengthView::mousePressEvent(QMouseEvent *event)
{
    QPointF scenePt = mapToScene(event->pos());
    qDebug() << scenePt.x();
    qDebug() << event->pos().x();
}
