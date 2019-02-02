#ifndef MEASURE_H
#define MEASURE_H

class NotationPage;
class Stave;
#define DEFAULT_STATE 100
#include <QObject>
#include <QList>
#include <QGraphicsLineItem>
#include "src/Notation/note.h"
class Measure
{
public:
    Measure();
    int prefferedMeasureWidth = 100;
    int width = prefferedMeasureWidth;
    QGraphicsLineItem *measureLine;
    std::map<float,QGraphicsItem*> notes;
    NotationPage * notationPage;
    void setPage(NotationPage *page);
    void setStave(Stave *stv);
    void insertNote(float beat,float value);
    int tsTop = 4;
    int tsBot = 4;
    const qreal minBorder = 10;
    int xInScene();
    Stave *stave;
};

#endif // MEASURE_H
