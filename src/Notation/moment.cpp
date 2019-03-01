#include "moment.h"
#include "note.h"
#include "chord.h"

Moment::Moment(Score* score) :Element (score)
{

}

void Moment::draw(QPainter *painter)
{
    for(auto& c: children())
    {
        c->draw(painter);
    }
}

elementType Moment::getType() const
{
    return MOMENT;
}
