#include "measure.h"
#include "src/Notation/notationpage.h"
Measure::Measure()
{
    measureLine = new QGraphicsLineItem(0,0,0,100);
}

void Measure::setPage(NotationPage *page)
{
    notationPage = page;
    measureLine->setLine(0,0,0,page->lineSpace * 4);
}
