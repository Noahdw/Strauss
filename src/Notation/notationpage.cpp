#include "notationpage.h"
#include <src/Notation/notationview.h>
NotationPage::NotationPage(NotationView *view) : notationView(view)
{
    setSizePolicy(QSizePolicy ::Expanding , QSizePolicy ::Expanding );
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setMinimumSize(1200,1500);
    scene = new QGraphicsScene;
    scene->setSceneRect(0,0,2480,3508);
    setScene(scene);
    fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
}

void NotationPage::addStave()
{
    for (int i = 0;i < 5; ++i)
    {
        scene->addLine(borderWidth,lastStavePos + (i *lineSpace),scene->width() - borderWidth,lastStavePos + (i *lineSpace));
    }
    staveWidth = scene->width() - 2 * borderWidth;
    lastStavePos += staveHeight + 100;
    fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    numStaves++;
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

void NotationPage::assignMeasure(Measure *measure)
{
    int stave =  measures.size() / staveGroupsPerPage();
    int offset = staveHeight + 100 * stave;
    int mes = (measures.size() - stave * staveGroupsPerPage()) % measuresPerWidth();
    if(stave >= numStaves)
        addStave();
    qDebug() <<"Stave "<< mes * prefferedMeasureWidth << ", MES " << staveHeight * stave + offset;
    measures[measure] = measures.size() + 1;
    measure->notes[0] = -4.0;
    measure->measureLine->setY(staveHeight * stave + offset);
    measure->measureLine->setX(mes * prefferedMeasureWidth + borderWidth);
    scene->addItem(measure->measureLine);
    measure->setPage(this);
    fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
}

void NotationPage::MousePressEvent(QMouseEvent *event)
{
     QGraphicsView::mousePressEvent(event);
}

void NotationPage::mouseDoubleClickEvent(QMouseEvent *event)
{
    QGraphicsView::mouseDoubleClickEvent(event);
}
