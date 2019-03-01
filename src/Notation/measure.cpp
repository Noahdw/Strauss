#include "measure.h"
#include <QPainter>
#include "src/Notation/note.h"
#include "src/Notation/n_common.h"
#include "moment.h"
#include "score.h"
Measure::Measure(Score* _score) : Element(_score)
{

}

void Measure::draw(QPainter *painter)
{

}

void Measure::insertStaff(int index)
{
    std::shared_ptr<QMap<float,Moment*>> staff;
    _moments.insert(index,staff);
}

void Measure::removeStaff(int index)
{
    Q_ASSERT(index < score()->numStaves());
}

QMap<float, Moment *> &Measure::moments(int staff)
{
    return *_moments[staff];
}

elementType Measure::getType() const
{
    return MEASURE;
}




