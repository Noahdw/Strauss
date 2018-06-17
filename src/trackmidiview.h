#ifndef TRACKMIDIVIEW_H
#define TRACKMIDIVIEW_H


class TrackView;

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <src/midimanager.h>

class TrackMidiView: public QGraphicsView{
    Q_OBJECT
public:
    TrackMidiView(QWidget* parent = 0);
    TrackView * trackView;
    void addViewItem(int start, int length, int note);
    void onPianoRollResized(float x);
    void deleteViewItem(int start, int note);
    void shareScene(QGraphicsScene * scene);

    int totalDT = MidiManager::TPQN * 50;

private:
    QGraphicsScene *scene;
    int highestNote = 0;
    int lowestNote = 127;

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // TRACKMIDIVIEW_H