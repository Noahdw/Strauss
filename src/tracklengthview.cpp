#include "tracklengthview.h"
#include "src/midimanager.h"
#include "src/audioengine.h"
#include "src/common.h"
#include "src/pianorollwidget.h"
#include "trackmidi.h"
#include "Controllers/pianorollcontainer.h"
/*
This class represents a multi-use bar above the piano roll. It displays
the current time of the track that is dependent on how zoomed in you
are in the piano roll. It also allows for a user to start playback from
a specific spot as well as to zoom the piano roll by dragging up/down.
*/
TrackLengthView::TrackLengthView(PianoRollContainer *p) : _container(p)
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

    // QPalette pal = palette();
    // pal.setColor(QPalette::Background,g_baseColor);
    //setAutoFillBackground(true);
    //setPalette(pal);
    setBackgroundRole(QPalette::NoRole);
}

void TrackLengthView::paintEvent(QPaintEvent *event)
{
    //scaleFactor = 1;//pianoRoll->scaleFactor;
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
void TrackLengthView::setScale(QMatrix matrix,qreal _scaleFactor)
{
    scaleFactor = _scaleFactor;
    setMatrix(matrix);
    viewport()->update();
}

void TrackLengthView::initTrackLengthView(QRectF sceneRect, float scaleX)
{
    setSceneRect(sceneRect);
    resetMatrix();
    scale(scaleX,1);
    viewport()->update();
}

void TrackLengthView::restoreTrack(TrackMidi *midiTrack)
{
    auto state = midiTrack->midiEditorState();
}
void TrackLengthView::mousePressEvent(QMouseEvent *event)
{
    QPointF scenePt = mapToScene(event->pos());
    qDebug() << scenePt.x();
    //This is a weird way of setting playback based on where you clicked in the scene
    //AudioManager will detect if the variable is -1 or not and will act appropriatly
    AudioEngine::requestedPlaybackPos = scenePt.x();
}

void TrackLengthView::resizeEvent(QResizeEvent *event)
{
    resetMatrix();
    float x = (float)width() / (scene->sceneRect().width());
    scale(x,1);
    horizontalScrollBar()->setValue(0);
    QGraphicsView::resizeEvent(event);
    //qDebug() << scaleFactor;

}

void TrackLengthView::showEvent(QShowEvent *event)
{
    fitInView(scene->sceneRect(),Qt::IgnoreAspectRatio);
    QGraphicsView::showEvent(event);
}

