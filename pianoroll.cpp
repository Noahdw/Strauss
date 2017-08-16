#include "pianoroll.h"
#include"pianorollitem.h"
#include <QDebug>
#include <functional>
#include <math.h>
int cols = 50;
int colSpacing = 0;
int tPQN = 120;
int totalDT = tPQN*cols;
double scaleFactor = 1;
PianoRoll::PianoRoll(QWidget *parent) : QGraphicsView(parent)
{

    sceneRect = new QRectF(0,0,800,PianoRollItem::keyHeight*127);
    update();

    setFixedSize(800,500);
    colSpacing = width()/cols;
   setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(ShowContextMenu(const QPoint &)));

}

void PianoRoll::mouseDoubleClickEvent(QMouseEvent  *event)
{
    if(event->button()==Qt::LeftButton)
    {
        QGraphicsItem *pRollTemp = itemAt(event->pos());
        if (pRollTemp!=0) {
            emit deleteNotesFromPROLL(pRollTemp);
        }
        else{
        QPointF mousePos = event->pos();

        int quadrant = mousePos.x() /(colSpacing * scaleFactor);
        int newY = mousePos.y() / PianoRollItem::keyHeight;
        quadrant = quadrant*scaleFactor;
        newY = newY*PianoRollItem::keyHeight;
        mousePos = mapToScene(quadrant*colSpacing,newY);
        // int newX = (((xx*tPQN*scaleFactor- 0) * NewRange) / OldRange) + 1;
        emit addNoteToPROLL(mousePos.x(),mousePos.y(),colSpacing*scaleFactor,quadrant*tPQN,tPQN*scaleFactor);

        QGraphicsView::mouseDoubleClickEvent(event);
        }
    }
}

void PianoRoll::mousePressEvent(QMouseEvent *event)
{

    if (event->button()==Qt::RightButton)
    {
        ShowContextMenu(event->pos());
    }
    QGraphicsView::mousePressEvent(event);
}
void PianoRoll::ShowContextMenu(const QPoint &pos)
{
    QMenu contextMenu(("Context menu"), this);

    QAction action1("2 bars", this);
    QAction action2("1 bars", this);
    QAction action3("1/2 bars", this);
    QAction action4("1/4 bars", this);
    QAction action5("1/8 bars", this);
    QAction action6("1/16 bars", this);

    connect(&action1,&QAction::triggered,this, [this]{scaleFactorChanged(8);});
    connect(&action2,&QAction::triggered,this, [this]{scaleFactorChanged(4);});
    connect(&action3,&QAction::triggered,this, [this]{scaleFactorChanged(2);});
    connect(&action4,&QAction::triggered,this, [this]{scaleFactorChanged(1);});
    connect(&action5,&QAction::triggered,this, [this]{scaleFactorChanged(0.5);});
    connect(&action6,&QAction::triggered,this, [this]{scaleFactorChanged(0.25);});


    contextMenu.addAction(&action1);
    contextMenu.addAction(&action2);
    contextMenu.addAction(&action3);
    contextMenu.addAction(&action4);
    contextMenu.addAction(&action5);
    contextMenu.addAction(&action6);


    contextMenu.exec(mapToGlobal(pos));
}
void PianoRoll::paintEvent(QPaintEvent *event)
{


    QPainter *painter = new QPainter(this->viewport());
    drawBackground(painter,*sceneRect);

      QGraphicsView::paintEvent(event);
      delete painter;
}
void PianoRoll::drawBackground(QPainter * painter, const QRectF & rect)
{
    QPen pen;
    pen.setColor(Qt::lightGray);
    pen.setWidthF(0.5);
    painter->setPen(pen);

    for (int var = 0; var <= 127; ++var)
    {
        painter->drawLine(0,var*PianoRollItem::keyHeight,this->viewport()->width(),var*PianoRollItem::keyHeight);
    }



    for (double var = 0; var < cols/scaleFactor; var+=1)
    {

        if(fmod(var,4/scaleFactor) == 0)
        {

            pen.setColor(Qt::black);
            painter->setPen(pen);
        }
        painter->drawLine(var*colSpacing*scaleFactor,0,var*colSpacing*scaleFactor,this->viewport()->height());
         pen.setColor(Qt::lightGray);
          painter->setPen(pen);
    }
}

void PianoRoll::scaleFactorChanged(double scale)
{
    scaleFactor = scale;



}

void PianoRoll::wheelEvent(QWheelEvent *event)
{

    this->viewport()->update();

}
