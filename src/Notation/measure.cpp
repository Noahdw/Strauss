#include "measure.h"
#include "src/Notation/notationpage.h"
#include "src/Notation/stave.h"
Measure::Measure()
{
    setMinimumWidth(100);
    setMinimumHeight(80);
    measureLine = new QGraphicsLineItem(0,0,0,100);
    scene = new QGraphicsScene(0,0,tsTop,20);
    setScene(scene);
    //setWindowFlag(Qt::FramelessWindowHint);
    setStyleSheet("background-color: rgba(0,0,0,0); border: 2px solid black;");
    setMouseTracking(true);
    cursorNote = new Note(n_selectedNoteValue);
    installEventFilter(this);
    scene->addItem(cursorNote);
    cursorNote->hide();
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHint(QPainter::Antialiasing);
}

void Measure::setPage(NotationPage *page)
{
    notationPage = page;
    measureLine->setLine(0,0,0,page->lineSpace * 4);
}

void Measure::setStave(Stave *stv)
{
    stave = stv;

}

void Measure::insertNote(float beat, float value)
{

    if(notes.count(beat) && false) // a note with same beat already exists
    {

    }
    else
    {
        if(value == DEFAULT_STATE)
        {
            auto * newNote = new Note(value);
            newNote->setX(tsTop/ 2);
            newNote->setY(5);
            notes[1] = newNote;
            scene->addItem(newNote);
            fitInView(scene->sceneRect(),Qt::IgnoreAspectRatio);

            return;
        }
        auto * newNote = new Note(value);
        auto oldNote = notes[beat];
        scene->removeItem(oldNote);
        notes[beat] = newNote;
        float totalDuration = 0;
        foreach(const auto &n, notes)
        {
            auto castN = dynamic_cast<Note*>(n.second);
            totalDuration += castN->noteValue;
        }
        float beatsNeeded = tsTop - totalDuration;

        float newVal = 0;
        while(beatsNeeded > 0)
        {
            auto *nn = new Note(newVal);
            notes[beat + newVal] = nn;
            scene->addItem(nn);
            nn->setPos(beat + newVal,5);
            beatsNeeded -= newVal;
            if(newVal == 0)
                newVal = value;
            else
                newVal *= 2.0;
        }

    }
}
// The note displayed under the mouse should snap to available notes in the scene
float Measure::getCurrentNoteUnderMouse(QPointF pos)
{

    float highest = 0;
    float last = 1;
    for(auto it = notes.begin();it != notes.end();it++)
    {
        if(it->first == DEFAULT_STATE)
            return 1.0;
        if(it->first > pos.x())
        {
            if(pos.x() - last <= (it->first - last) / 2)
                return last;
            return it->first;
        }
        if(it->first > highest)
            highest = it->first;
        last = it->first;
    }
    return highest;
}

float Measure::getPitchUnderMouse(QPointF pos)
{
    return pos.y();
}

int Measure::xInScene()
{
    return measureLine->x();
}

void Measure::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    insertNote(getCurrentNoteUnderMouse(event->pos()),0.5);
    fitInView(scene->sceneRect(),Qt::IgnoreAspectRatio);
}

void Measure::mouseMoveEvent(QMouseEvent *event)
{
    auto pos = mapToScene(event->pos());
    auto x = getCurrentNoteUnderMouse(pos);
    auto y = getPitchUnderMouse(pos);
    cursorNote->setPos(x,y);
    notationPage->forceRedraw();
    QGraphicsView::mouseMoveEvent(event);

}

void Measure::showEvent(QShowEvent *event)
{
    QGraphicsView::showEvent(event);
}

bool Measure::eventFilter(QObject *obj, QEvent *event)
{
    // What a dumb hack, I can't fitInView until widget is shown BUT SHOW EVENT IS APPERENTLY TOO EARLY??
    if(event->type() == QEvent::UpdateLater)
    {
        fitInView(scene->sceneRect(),Qt::IgnoreAspectRatio);
    }

    if (event->type() == QEvent::Enter)
    {
        auto mouse = static_cast<QMouseEvent*>(event);
        cursorNote->show();
        cursorNote->setPos(mapToScene(mouse->pos()));
        notationPage->forceRedraw();
    }
    else if (event->type() == QEvent::Leave)
    {
        auto mouse = static_cast<QMouseEvent*>(event);
        cursorNote->hide();
        cursorNote->setPos(mapToScene(mouse->pos()));
        notationPage->forceRedraw();

    }
    return QWidget::eventFilter(obj, event);

}



