#include "tracklengthview.h"
#include "src/midimanager.h"
#include "src/audiomanager.h"
#include "src/common.h"

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

    scene = new QGraphicsScene(0,0,MidiManager::TPQN*g_quarterNotes,height());
    scene->setSceneRect(0,0,MidiManager::TPQN*g_quarterNotes,height());
    this->setScene(scene);

    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::lightGray);
    setAutoFillBackground(true);
    setPalette(pal);
    setBackgroundRole(QPalette::NoRole);
}

void TrackLengthView::paintEvent(QPaintEvent *event)
{
    QPainter painter(viewport());
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidthF(1);
    painter.setPen(pen);
    painter.resetMatrix();
    painter.setBrush(Qt::black);
    double adjust = 5.0;
    for (int var = 0; var < g_quarterNotes / scaleFactor; var++)
    {

        if (var % 4 == 0)
        {

            painter.drawText(var*(float)MidiManager::TPQN * transform().m11() * scaleFactor - horizontalScrollBar()->value() + adjust ,height()/2,QString::number((float)var/4.0*scaleFactor));
            painter.drawLine(var*(float)MidiManager::TPQN * transform().m11() * scaleFactor - horizontalScrollBar()->value(),0,var*MidiManager::TPQN * transform().m11()
                             * scaleFactor - horizontalScrollBar()->value(),height()/2);
        }
        else
        {
            painter.drawLine(var*(float)MidiManager::TPQN * transform().m11() * scaleFactor - horizontalScrollBar()->value(),0,var*MidiManager::TPQN * transform().m11()
                             * scaleFactor - horizontalScrollBar()->value(),height()/4);
        }

    }
}

void TrackLengthView::mouseDoubleClickEvent(QMouseEvent *event)
{

}
void TrackLengthView::setScale(float x, bool needsReset, int wheelPos, double _scaleFactor)
{
    scaleFactor = _scaleFactor;
    if (needsReset) {
        resetMatrix();
    }
    scale(x,1);
    horizontalScrollBar()->setValue(wheelPos);
    viewport()->update();

}

void TrackLengthView::initTrackLengthView(QRectF sceneRect, float scaleX)
{
    setSceneRect(sceneRect);
    resetMatrix();
    setScale(scaleX,true,0,1);
    viewport()->update();
}
void TrackLengthView::mousePressEvent(QMouseEvent *event)
{
    QPointF scenePt = mapToScene(event->pos());
   qDebug() << scenePt.x();
   //This is a weird way of setting playback based on where you clicked in the scene
   //AudioManager will detect if the variable is -1 or not and will act appropriatly
    AudioManager::requestedPlaybackPos = scenePt.x();
}

void TrackLengthView::resizeEvent(QResizeEvent *event)
{
     fitInView(scene->sceneRect());
     QGraphicsView::resizeEvent(event);
     viewport()->update();
}

