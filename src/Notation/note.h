#ifndef NOTE_H
#define NOTE_H

#include <QObject>
#include <QPainter>
#include "element.h"
#include "src/Notation/n_common.h"
class Note : public Element
{
public:
    Note(Score* score =0);

};

#endif // NOTE_H
