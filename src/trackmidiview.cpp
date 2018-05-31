#include "trackmidiview.h"
#include <src/trackview.h>
#include <src/pianorollitem.h>

TrackMidiView::TrackMidiView(QWidget *parent)  : QGraphicsView(parent)
{

    scene = new QGraphicsScene;
    scene->setSceneRect(0,0,totalDT,12*128);
   setScene(scene);


    setMinimumHeight(70);
    setMinimumWidth(1000);
    setMaximumHeight(70);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scale(((float)width() / totalDT),(float)height()/1536);



}

void TrackMidiView::addViewItem(int start, int length, int note)
{
    QGraphicsRectItem *rect = new QGraphicsRectItem(0,0,length,50);
    scene->addItem(rect);
    qDebug() << start << "    " << note;
   //  QPen pen(Qt::black,0);
   // rect->setPen(pen);
    rect->setBrush(Qt::darkGray);
    rect->setX(start);
    rect->setY(note);
    qDebug() << "recieved";
}

void TrackMidiView::paintEvent(QPaintEvent *event)
{
    QPainter *painter = new QPainter(this->viewport());
    QPen pen;
    pen.setColor(Qt::black);
    painter->setBrush(Qt::lightGray);

    painter->setPen(pen);
    painter->drawRect(viewport()->rect());
    QGraphicsView::paintEvent(event);

}
void TrackMidiView::onPianoRollResized(float x)
{
    resetMatrix();
    scale(x,(float)height()/1536);
}

void TrackMidiView::deleteViewItem(int start, int note)
{
    QGraphicsItem * item = scene->itemAt(QPointF(start,note),QGraphicsView::transform());
    QGraphicsRectItem * ritem = static_cast<QGraphicsRectItem*>(item);
    qDebug() << start << "    " << note;
    scene->removeItem(ritem);
    delete(ritem);
}
