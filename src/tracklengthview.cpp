#include "tracklengthview.h"
#include "src/midimanager.h"
#include "src/audiomanager.h"
/*
This class represents a multi-use bar above the piano roll. It displays
the current time of the track that is dependent on how zoomed in you
are in the piano roll. It also allows for a user to start playback from
a specific spot as well as to zoom the piano roll by dragging up/down.
*/
TrackLengthView::TrackLengthView(QWidget *parent) : QGraphicsView(parent)
{
   // setViewportUpdateMode(MinimalViewportUpdate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setContextMenuPolicy(Qt::CustomContextMenu);
    setMinimumWidth(1000);
    setMinimumHeight(20);
    setMaximumHeight(20);
    scene = new QGraphicsScene;
    scene->setSceneRect(0,0,MidiManager::TPQN*60,height());
    this->setScene(scene);
   (scene->sceneRect(),Qt::KeepAspectRatio);
  //  resetMatrix();this->scale(((float)width() / (MidiManager::TPQN*60)),1);
}

void TrackLengthView::paintEvent(QPaintEvent *event)
{
    QPainter painter(viewport());
    QPen pen;
    pen.setColor(Qt::lightGray);
    painter.setBrush(Qt::lightGray);
    painter.setPen(pen);
    painter.drawRect(viewport()->rect());
    painter.setBrush(Qt::black);
    double cols = width()/MidiManager::TPQN;
    for (int var = 0; var < cols; var++)
    {
        painter.drawRect(var*cols,0,3,height()/4);
    }
}

void TrackLengthView::mouseDoubleClickEvent(QMouseEvent *event)
{

}
void TrackLengthView::setScale(float x, bool needsReset, int wheelPos)
{
    if (needsReset) {
        resetMatrix();
    }
    scale(x,1);
    QScrollBar *wheel;
    wheel = horizontalScrollBar();
    wheel->setValue(wheelPos);

}
void TrackLengthView::mousePressEvent(QMouseEvent *event)
{
    QPointF scenePt = mapToScene(event->pos());
   qDebug() << scenePt.x();
   //This is a weird way of setting playback based on where you clicked in the scene
   //AudioManager will detect if the variable is -1 or not and will act appropriatly
    AudioManager::requestedPlaybackPos = scenePt.x();
}
