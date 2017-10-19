#ifndef TRACKMIDIVIEW_H
#define TRACKMIDIVIEW_H


class TrackView;

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <midimanager.h>

class TrackMidiView: public QGraphicsView{
    Q_OBJECT
public:
    TrackMidiView(QWidget* parent = 0);
    TrackView * trackView;
    void updateViewItems(int start, int length, int note);
    int totalDT = MidiManager::TPQN * 50;
    void onPianoRollResized(float x);

private:
    QGraphicsScene *scene;
    int highestNote = 0;
    int lowestNote = 127;

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // TRACKMIDIVIEW_H
