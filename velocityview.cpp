#include "velocityview.h"
#include <midimanager.h>
#include <QGraphicsRectItem>
#include <pianorollitem.h>
#include <QScrollBar>
   int viewHeight = 100;
VelocityView::VelocityView(QWidget *parent) : QGraphicsView(parent)
{
    setFixedSize(1400,100);


    //setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
   // setViewportUpdateMode(MinimalViewportUpdate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setContextMenuPolicy(Qt::CustomContextMenu);

    scene = new QGraphicsScene;
    scene->setSceneRect(0,0,MidiManager::TPQN*50,viewHeight);
    this->setScene(scene);
    this->scale(((float)width() / (MidiManager::TPQN*50)),1);

}

void VelocityView::updateItems(int start, int velocity, int note, bool adding)
{
 qDebug() << "update recieved";
 if (adding) {
  QGraphicsRectItem *line = new QGraphicsRectItem(0,0,1,velocity);
     line->setCacheMode(QGraphicsItem::NoCache);
      scene->addItem(line);
      QPen pen(Qt::black,0);
      line->setPen(pen);
      line->setBrush(Qt::black);
     line->setPos(start,viewHeight - velocity);
     scene->update(0,0,MidiManager::TPQN*50,100);


 }
}

void VelocityView::setScale(float x, bool needsReset, int wheelPos)
{
    if (needsReset) {
        resetMatrix();
    }

     this->scale(x,1);
    QScrollBar *wheel;
    wheel = this->horizontalScrollBar();
    wheel->setValue(wheelPos);
}

void VelocityView::paintEvent(QPaintEvent *event)
{
      QPainter *painter = new QPainter(this->viewport());
      QPen pen;
      pen.setColor(Qt::lightGray);
      painter->setBrush(Qt::lightGray);

      painter->setPen(pen);
      painter->drawRect(viewport()->rect());
       QGraphicsView::paintEvent(event);
       delete painter;
}
