#ifndef STAVE_H
#define STAVE_H

#include <QObject>
#include <QGraphicsLineItem>
#include <src/Notation/measure.h>
class Stave
{
public:
    Stave();
    QGraphicsLineItem* horizontalLines[5];
    QVector<Measure*> measures;
    int height;
    int yInScene;
    int priority;
    int lineHeight;
};

#endif // STAVE_H
