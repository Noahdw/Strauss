#include "score.h"
#include "measure.h"
#include "stave.h"


Score::Score()
{
    _pageSettings = {100,100,100,100};
}

void Score::addStave(int index)
{
    _staves.insert(index,Stave());
}

void Score::addMeasure(int index)
{
    _measure.insert(index,std::make_shared<Measure>());
}
