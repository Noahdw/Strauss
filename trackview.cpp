#include "trackview.h"
#include "QGraphicsRectItem"
#include <QGraphicsItem>
TrackView::TrackView(QWidget *parent) : QGraphicsView(parent)
{
    scene = new QGraphicsScene();
    this->setScene(scene);
}

void TrackView::trackViewChanged(mSong *song)
{

}


