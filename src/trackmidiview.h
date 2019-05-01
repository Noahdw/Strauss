#ifndef TRACKMIDIVIEW_H
#define TRACKMIDIVIEW_H


class TrackWidget;

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <src/midimanager.h>

class TrackMidiView: public QGraphicsView{
    Q_OBJECT
public:
    TrackMidiView(QWidget* parent = 0);
    TrackWidget * trackView;
    void onPianoRollResized(float x);
    void clickedOn(bool state);
    int totalDT = MidiManager::TPQN * 50;


private:
    int highestNote = 0;
    int lowestNote = 127;
    QBrush brush;
    QPen pen;

protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // TRACKMIDIVIEW_H
