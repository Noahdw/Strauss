#ifndef ELEMENT_H
#define ELEMENT_H

#include <QObject>
#include <QPainter>

class Score;

enum elementType{
    NONE,
    NOTE,
    CHORD,
    MEASURE,
    PAGE,
    MOMENT
};

class Element
{
public:
    Element(Score* score =0);
    Score* score(){return _score;}
    virtual void draw(QPainter*){}
    virtual void add(Element*){}
    virtual void setParent(Element* parent){_parent = parent;}
    virtual void remove(Element*){}
    virtual QRectF size() {return QRectF(0,0,0,0);}
    virtual elementType getType() const {return NONE;}
    virtual Element* parent() {return _parent;}
    int X;
    int Y;
private:
    Score* _score;
    Element* _parent;
};

#endif // ELEMENT_H
