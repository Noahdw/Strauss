#ifndef CHORD_H
#define CHORD_H

#include "element.h"
class Score;

class Chord : public Element
{
public:
    Chord(Score* score =0);
    elementType getType() const;
};

#endif // CHORD_H
