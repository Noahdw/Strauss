#ifndef PAGE_H
#define PAGE_H

#include "element.h"
class Score;
class Measure;

class Page : public Element
{
public:
    Page(Score* score =0);
    void draw(QPainter *painter);
    QList<std::shared_ptr<Measure>> &measures() {return _measures;}
    QRectF size();
private:
    QList<std::shared_ptr<Measure>> _measures;
};

#endif // PAGE_H
