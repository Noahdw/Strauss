#ifndef MOMENT_H
#define MOMENT_H

#include "element.h"

class Measure;
class Chord;
class Note;

class Moment : public Element
{
public:
    Moment(Score *score =0);
    void draw(QPainter* painter);
    QList<std::shared_ptr<Element>>& children() {return _children;}
    elementType getType() const;
private:
    QList<std::shared_ptr<Element>> _children;
};

#endif // MOMENT_H
