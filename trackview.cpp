#include "trackview.h"
#include "QGraphicsRectItem"
#include <QGraphicsItem>
#include <pianorollitem.h>
#include <QDebug>
TrackView::TrackView(mTrack track,QWidget *parent) : QFrame(parent)
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
    setFixedSize(widgetWidth,70);
    setFrameStyle(QFrame::Box | QFrame::Raised);
    setLineWidth(0);
    setMidLineWidth(0);

}

void TrackView::paintEvent(QPaintEvent *event)
{
    QPainter *painter = new QPainter(this);

    QBrush brush(Qt::lightGray);
    painter->setBrush(brush);

    painter->drawRect(0,0,widgetWidth,70);

}

void TrackView::mousePressEvent(QMouseEvent *event)
{
    //
   emit trackClickedOn(id);
    qDebug() << id;
}





