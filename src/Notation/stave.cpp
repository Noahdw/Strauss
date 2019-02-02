#include "stave.h"

Stave::Stave()
{
    for (int i = 0; i < 5; ++i)
    {
        horizontalLines[i] = new QGraphicsLineItem(0,0,100,0);
    }
}
