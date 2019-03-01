#ifndef MEASURE_H
#define MEASURE_H

class Stave;
class Score;
class Moment;
#include <QObject>

#include <QDebug>
#include "element.h"

class Measure : public Element
{
public:
    Measure(Score* _score =0);
    void draw(QPainter* painter);
    void insertStaff(int index);
    void removeStaff(int index);
    QMap<float, Moment *> &moments(int staff);
    elementType getType() const;
private:
    QVector<std::shared_ptr<QMap<float, Moment *> > > _moments;
};

#endif // MEASURE_H
