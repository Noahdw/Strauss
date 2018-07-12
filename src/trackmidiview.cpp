#include "trackmidiview.h"
#include <src/trackview.h>
#include <src/pianorollitem.h>

TrackMidiView::TrackMidiView(QWidget *parent)  : QGraphicsView(parent)
{

    setViewportUpdateMode(MinimalViewportUpdate);
    setRenderHint(QPainter::Antialiasing);

  //  setMinimumWidth(400);
    setMinimumHeight(70);
   setMaximumHeight(100);
 //   setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void TrackMidiView::addViewItem(int start, int length, int note)
{
    //    QGraphicsRectItem *rect = new QGraphicsRectItem(0,0,length,50);
    //    //scene->addItem(rect);
    //    qDebug() << start << "    " << note;
    //   //  QPen pen(Qt::black,0);
    //   // rect->setPen(pen);
    //    rect->setBrush(Qt::darkGray);
    //    rect->setX(start);
    //    rect->setY(note);
    //    qDebug() << "recieved";
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

void TrackMidiView::deleteViewItem(int start, int note)
{
    //    QGraphicsItem * item = scene->itemAt(QPointF(start,note),QGraphicsView::transform());
    //    QGraphicsRectItem * ritem = static_cast<QGraphicsRectItem*>(item);
    //    qDebug() << start << "    " << note;
    //    scene->removeItem(ritem);
    //    delete(ritem);
}

void TrackMidiView::shareScene(QGraphicsScene *scene)
{
    this->scene = scene;
    setScene(scene);
    totalDT = scene->width();
    fitInView(scene->sceneRect(),Qt::IgnoreAspectRatio);

}
