#ifndef NOTATIONVIEW_H
#define NOTATIONVIEW_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <src/mastertrack.h>
#include <src/trackmidi.h>
#include <QScrollBar>
#include <src/Notation/notationpage.h>
#include <QGraphicsProxyWidget>
#include <src/Notation/measure.h>
class NotationView : public QGraphicsView
{
    Q_OBJECT
public:
    NotationView(MasterTrack *master);
    NotationPage *AddPage(int index);
    void AddInstrument(TrackMidi *track);
    void AddMeasure(int index);
    int numInstruments();
    void assignMeasureToPage();
    void changeCurrentNote(float value);
protected:
    void wheelEvent(QWheelEvent *event);
private:
    QGraphicsScene *scene;
    QList<TrackMidi*> instruments;
    QList<NotationPage*> pages;
    QList<Measure*> measures;
    MasterTrack *masterTrack;
    int defaultMeaures = 60;
    float xscale = 1.1;

};

#endif // NOTATIONVIEW_H
