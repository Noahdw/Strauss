#ifndef STAVE_H
#define STAVE_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QGraphicsLineItem>
#include <src/Notation/measure.h>
class Stave : public QFrame
{
    Q_OBJECT
public:

    Stave();
    QVector<Measure*> measures;

    int yInScene;
    int priority;
    int numLines = 5;
    int lineHeight = 20;
    void assignMeasure(Measure * measure);
    void paintEvent(QPaintEvent * event);
    QHBoxLayout *measureLayout;
};

#endif // STAVE_H
