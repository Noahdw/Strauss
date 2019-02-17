#include "notationview.h"

NotationView::NotationView(MasterTrack *master) :masterTrack(master)
{
    setMinimumSize(2000,900);
    scene = new QGraphicsScene;
    cursorNote = new Note(1);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
 //   setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  //  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHint(QPainter::Antialiasing);
    setScene(scene);
    scene->addItem(cursorNote);
    cursorNote->setZValue(1);
}

NotationPage *NotationView::AddPage(int index)
{
    NotationPage *page = new NotationPage(this);
    auto proxy = scene->addWidget(page);
    proxy->setX(index * 1200);
    qDebug() << scene->sceneRect();
    pages.append(page);

    fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    return page;
}

void NotationView::AddInstrument(TrackMidi *track)
{
    instruments.push_back(track);
    foreach(auto & page , pages)
    {
        page->addStave();
    }

    if(pages.size() == 0)
    {
        auto page = AddPage(0);
        page->addStave();

    }

}

void NotationView::AddMeasure(int index)
{
    Measure *measure = new Measure;
    measures.append(measure);
    int mes = 1200 / measure->prefferedMeasureWidth;

    foreach(auto& page, pages)
    {
        qDebug() << page->unusedMeasures();
        if(page->unusedMeasures() > 0)
        {
            page->assignMeasure(measure);
            return;
        }
    }

}

int NotationView::numInstruments()
{
    return instruments.size();
}

void NotationView::assignMeasureToPage()
{
    for(int i = 0; i < defaultMeaures; ++i)
    {
        AddMeasure(i);
    }
}

void NotationView::changeCurrentNote(float value)
{
    for(const auto & page : pages)
    {
      //  auto m = page->getMeasures();
    }
}

void NotationView::setCursorNotePos(QPoint pos)
{
    cursorNote->setPos(mapToScene(pos));
    cursorNote->setY(cursorNote->y() - verticalScrollBar()->value());
   // cursorNote->set
}

void NotationView::wheelEvent(QWheelEvent *event)
{
    if(event->modifiers().testFlag(Qt::ControlModifier))
    {
        const QPointF p0scene = mapToScene(event->pos());

        qreal factor = std::pow(1.1,event->angleDelta().y() /120);
        scale(factor, factor);

        const QPointF p1mouse = mapFromScene(p0scene);
        const QPointF move = p1mouse - event->pos(); // The move
        horizontalScrollBar()->setValue(move.x() + horizontalScrollBar()->value());
        verticalScrollBar()->setValue(move.y() + verticalScrollBar()->value());
    }
    else
    {

        verticalScrollBar()->setValue(verticalScrollBar()->value() - event->angleDelta().y() /120*14);
    }
}


