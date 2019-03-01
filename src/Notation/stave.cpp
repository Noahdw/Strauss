#include "stave.h"
#include "measure.h"
#include "score.h"

Stave::Stave(Score *score) : Element(score)
{

}

QRectF Stave::size()
{
    int w =score()->pageWidth - score()->pageSettings().lBorderWidth - score()->pageSettings().rBorderWidth;
    int h = staveLines() * lineSpacing();
    return QRectF(X,Y,w,h);
}


