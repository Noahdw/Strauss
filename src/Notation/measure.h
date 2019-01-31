#ifndef MEASURE_H
#define MEASURE_H

class NotationPage;

#include <QObject>
#include <QGraphicsLineItem>
class Measure
{
public:
    Measure();
    int prefferedMeasureWidth = 100;
    int width = prefferedMeasureWidth;
    QGraphicsLineItem *measureLine;
    std::map<float,float> notes;
    NotationPage * notationPage;
    void setPage(NotationPage *page);
};

#endif // MEASURE_H
