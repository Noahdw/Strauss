#include "chord.h"

Chord::Chord(Score* score) : Element (score)
{

}

elementType Chord::getType() const
{
    return CHORD;
}
