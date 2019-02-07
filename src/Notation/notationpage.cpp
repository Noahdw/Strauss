#include "notationpage.h"
#include <src/Notation/notationview.h>
NotationPage::NotationPage(NotationView *view) : notationView(view)
{
    setSizePolicy(QSizePolicy ::Expanding , QSizePolicy ::Expanding );
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setMinimumSize(1200,1500);
    scene = new QGraphicsScene;
    scene->setSceneRect(0,0,2480,3508);
    setScene(scene);

}

Stave *NotationPage::addStave()
{
    auto *stave = new Stave;
    stave->setMaximumWidth(scene->width() - borderWidth);
    stave->setMinimumWidth(scene->width() - borderWidth);
    auto proxy = scene->addWidget(stave);
    proxy->setX(borderWidth);
    proxy->setY(lastStavePos);
    staveWidth = scene->width() - 2 * borderWidth;
    lastStavePos += staveHeight + 100;
    fitInView(scene->sceneRect(),Qt::KeepAspectRatio);

    stave->priority = staves.size();
    staves.append(stave);
    numStaves++;
    return stave;
}

int NotationPage::unusedMeasures()
{
    return staveGroupsPerPage() * measuresPerWidth() - measures.size();
}

int NotationPage::staveGroupsPerPage()
{
    // qDebug() << "PP " << height();
    return (scene->height() - staveHeight) / ((notationView->numInstruments()) * (staveHeight + 100));
}

int NotationPage::measuresPerWidth()
{
    return staveWidth / prefferedMeasureWidth;
}

int NotationPage::staveYFromMeasure(Measure *measure)
{
    return staves[measures[measure]]->yInScene;
}

void NotationPage::assignMeasure(Measure *measure)
{
    int stave =  measures.size() / staveGroupsPerPage();
    int offset = staveHeight + 100 * stave;
    int mes = (measures.size() - stave * staveGroupsPerPage()) % measuresPerWidth();
    if(stave >= numStaves)
        addStave();
    qDebug() <<"Stave "<< mes * prefferedMeasureWidth << ", MES " << staveHeight * stave + offset;
    measures[measure] = measures.size() + 1;

    measure->setPage(this);
    measure->setStave(staves[stave]);
    staves[stave]->assignMeasure(measure);
    staves[stave]->measures.append(measure);
    measure->insertNote(1,100);
    fitInView(scene->sceneRect(),Qt::KeepAspectRatio);

}

void NotationPage::forceRedraw()
{
    viewport()->update();
}



void NotationPage::MousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    auto scenePos = mapToScene(event->pos());
    auto stave = getStaveFromScene(scenePos);
    auto measure = getMeasureFromScene(stave,scenePos);
    if(!measure)  qDebug() << "Measure null";
    qDebug() << (measure->measureLine->x());
    measure->insertNote(1,2);
    qDebug() << (measure->notes.size());
}





Stave *NotationPage::getStaveFromScene(QPointF pos)
{
    foreach (auto &s, staves)
    {
        if(s->yInScene > pos.y())
            return s;
    }
    return staves[0];
}

Measure *NotationPage::getMeasureFromScene(Stave *stave, QPointF pos)
{
    int total = 0;
    foreach(auto & m, stave->measures)
    {


        if(m->xInScene() > pos.x())
        {
            return m;
        }
    }
    return NULL;
}
