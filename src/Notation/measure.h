#ifndef MEASURE_H
#define MEASURE_H

class NotationPage;
class Stave;
#define DEFAULT_STATE 100
#include <QObject>
#include <QList>
#include <QGraphicsLineItem>
#include <QDebug>
#include <QGraphicsView>
#include "src/Notation/note.h"
#include "src/Notation/n_common.h"
class Measure : public QGraphicsView
{
public:
    Measure();
    int prefferedMeasureWidth = 100;

    QGraphicsLineItem *measureLine;
    std::map<float,QGraphicsItem*> notes;
    NotationPage * notationPage;
    void setPage(NotationPage *page);
    void setStave(Stave *stv);
    void insertNote(float beat,float value);
    float getCurrentNoteUnderMouse(QPointF pos);
    float getPitchUnderMouse(QPointF pos);
    int tsTop = 4;
    int tsBot = 4;
    const qreal minBorder = 10;
    int xInScene();
    Stave *stave;

        bool eventFilter(QObject *obj, QEvent *event);
     QGraphicsScene *scene;
 protected:
     void mousePressEvent(QMouseEvent *event);
     void mouseMoveEvent(QMouseEvent *event);
     void showEvent(QShowEvent *event);
private:
    Note *cursorNote;
};

#endif // MEASURE_H
