#include "trackmidiview.h"
#include <trackview.h>
#include <pianorollitem.h>

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

void TrackMidiView::updateViewItems(int start, int length, int note)
{
    QGraphicsRectItem *rect = new QGraphicsRectItem(0,0,length,12);
    scene->addItem(rect);

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
