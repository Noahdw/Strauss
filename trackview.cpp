#include "trackview.h"
#include "QGraphicsRectItem"
#include <QGraphicsItem>
#include <pianorollitem.h>
#include <QDebug>
TrackView::TrackView(mTrack track,QWidget *parent) : QWidget(parent)
{
    this->track = track;
    instrumentName = track.instrumentName;
    if (instrumentName == "") {
        instrumentName = "new track";
    }
    instrumentLabel = new QLabel(instrumentName);
    instrumentLabel->setMaximumWidth(widgetWidth-10);

    vlayout = new QVBoxLayout;
    vlayout->addWidget(instrumentLabel);
    setLayout(vlayout);
    setFixedSize(widgetWidth,40);

}

void TrackView::paintEvent(QPaintEvent *event)
{
    QPainter *painter = new QPainter(this);
    QPen pen(Qt::black);
    QBrush brush(Qt::gray);
    painter->setBrush(brush);
    painter->setPen(pen);
    painter->drawRect(0,0,widgetWidth,40);

}

void TrackView::mousePressEvent(QMouseEvent *event)
{
   emit trackClickedOn(id);
    qDebug() << id;
}





