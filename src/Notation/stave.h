#ifndef STAVE_H
#define STAVE_H

#include <QObject>
#include <QWidget>
#include "element.h"

class Measure;
class Score;

class Stave : public Element
{
public:
    Stave(Score* score =0);
    QRectF size();
    int staveLines()  const {return _staveLines;}
    int lineSpacing() const {return _lineSpacing;}


private:
    int _staveLines = 5;
    int _lineSpacing = 20;
};

#endif // STAVE_H
