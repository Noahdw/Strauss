#include "page.h"
#include <QPainter>
#include "score.h"
#include "measure.h"

Page::Page(Score *score) : Element (score)
{

}

void Page::draw(QPainter *painter)
{

}

QRectF Page::size()
{
    return QRectF(X,Y,score()->pageWidth,score()->pageHeight);
}
