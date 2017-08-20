#include "pianoroll.h"
#include"pianorollitem.h"
#include <QDebug>
#include <functional>
#include <math.h>
#include <QScrollBar>
#include <QList>

QList<PianoRollItem*> activeNotes;
double scaleFactor = 1;

PianoRoll::PianoRoll(QWidget *parent) : QGraphicsView(parent)
{
    setFixedSize(1200,400);


    setViewportUpdateMode(FullViewportUpdate);

    colSpacing = width()/cols;
    sceneRect = new QRectF(0,0,colSpacing*cols,PianoRollItem::keyHeight*127);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(ShowContextMenu(const QPoint &)));

    viewport()->update();
}

void PianoRoll::mouseDoubleClickEvent(QMouseEvent  *event)
{
    if(event->button()==Qt::LeftButton)
    {

        QGraphicsItem *pRollTemp = itemAt(event->pos());
        if (pRollTemp!=0) {
            //Adds an existing note from song
            emit deleteNotesFromPROLL(pRollTemp);
            activeNotes.clear();
        }
        else{
            //Adds a new note to song
            QPointF mousePos = mapToScene(event->pos());

            int quadrant = mousePos.x() /(colSpacing * scaleFactor);
            int newY = mousePos.y() / PianoRollItem::keyHeight;

            quadrant = quadrant*scaleFactor*colSpacing;
            newY = newY*PianoRollItem::keyHeight;

            emit addNoteToPROLL(quadrant,newY,colSpacing*scaleFactor,quadrant*tPQN/colSpacing,tPQN*scaleFactor);

            QGraphicsView::mouseDoubleClickEvent(event);
        }
    }
}

//Shows context menu
void PianoRoll::mousePressEvent(QMouseEvent *event)
{

    if (event->button()==Qt::RightButton)
    {
        ShowContextMenu(event->pos());
        return;
    }
    QGraphicsItem *pNote = itemAt(event->pos());

    //If a note was clicked on
    if(pNote!=0) {
        //Shift allows multiple selection of notes
        if (!event->modifiers().testFlag(Qt::ShiftModifier)) {
            clearActiveNotes();
        }

        PianoRollItem *castNote = dynamic_cast<PianoRollItem*>(pNote);
        castNote->brush = QBrush(Qt::darkCyan);
        castNote->update(castNote->boundingRect());
        activeNotes.append(castNote);
    }
    //just the view/canvas clicked on
    else if(activeNotes.length() > 0){
        clearActiveNotes();
    }
    QGraphicsView::mousePressEvent(event);
}
void PianoRoll::clearActiveNotes()
{
    foreach (auto note, activeNotes) {
        PianoRollItem *castNote = dynamic_cast<PianoRollItem*>(note);
        if (castNote==0) {
            continue;
        }
        castNote->brush = QBrush(QColor(102, 179, 255));
        castNote->update(castNote->boundingRect());
    }
    activeNotes.clear();
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
    //Handles painting in background so as not to overlap notes
    QPainter *painter = new QPainter(this->viewport());
    drawBackground(painter,*sceneRect);

    QGraphicsView::paintEvent(event);
    delete painter;
}
bool shouldPaint = true;

void PianoRoll::drawBackground(QPainter * painter, const QRectF & rect)
{
    QPen pen;
    pen.setColor(Qt::lightGray);
    pen.setWidthF(0.5);
    painter->setPen(pen);
    painter->eraseRect(*sceneRect);
    //Draws the horizontals lines
    for (int var = 0; var <= 127; ++var)
    {

        painter->drawLine(horizontalScrollBar()->value(),var*PianoRollItem::keyHeight,width()+horizontalScrollBar()->value(),var*PianoRollItem::keyHeight);
    }

    //Draws the vertical lines
    for (double var = 0; var < cols/scaleFactor; var+=1)
    {
        //Every whole note(if in scale) is drawn black
        if(fmod(var,4/scaleFactor) == 0)
        {
            pen.setColor(Qt::black);
            painter->setPen(pen);
        }
        painter->drawLine(var*colSpacing*scaleFactor,verticalScrollBar()->value(),var*colSpacing*scaleFactor,verticalScrollBar()->value()+height());

        pen.setColor(Qt::lightGray);
        painter->setPen(pen);
    }
}


void PianoRoll::scaleFactorChanged(double scale)
{
    scaleFactor = scale;
    this->viewport()->update();
}
int yscroller = 0;

void PianoRoll::wheelEvent(QWheelEvent *event)
{
    QScrollBar* wheelPos;
    if(event->modifiers().testFlag(Qt::ControlModifier))
    {

        colSpacing+= event->delta()/120;

        if (colSpacing*cols < width()) {
            colSpacing = (float)width() / cols;

            if (colSpacing==0) {
                colSpacing = 5;
            }
        }
        this->viewport()->update();
        QRectF visibleRect(horizontalScrollBar()->value(),verticalScrollBar()->value(),width(),height());
        emit changeSceneRect(QRectF(0,0,cols*colSpacing,PianoRollItem::keyHeight*127),sceneRect,visibleRect );

        sceneRect = new QRectF(0,0,cols*colSpacing,PianoRollItem::keyHeight*127);

        int zoom = event->angleDelta().y() /120;
        wheelPos=this->horizontalScrollBar();
        int newX = floor((event->pos().x()+horizontalScrollBar()->value()) / colSpacing);

        wheelPos->setValue(horizontalScrollBar()->value()+(newX * zoom));


    }
    else
    {
        yscroller=event->angleDelta().y() /120*11;
        wheelPos=this->verticalScrollBar();
        wheelPos->setValue(verticalScrollBar()->value()-yscroller);
        QMatrix matrix;
           matrix.scale(2, 2);
          // matrix.rotate(rotateSlider->value());

           this->setMatrix(matrix);
    }
}

//Called by mainwindow to update this view
void PianoRoll::notifyViewChanged(int tpqn,int cols)
{
    this->cols = cols;
    this->tPQN = tpqn;
    this->colSpacing = (double)width()/cols;
    if (colSpacing<2) {
        scaleFactor = 16;
    }
    qDebug() <<"Cols: " << cols;
    qDebug() <<"colSpacing: " << colSpacing;
    this->viewport()->update();
    this->update();

}

