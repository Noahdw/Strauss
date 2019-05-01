#ifndef VELOCITYVIEW_H
#define VELOCITYVIEW_H
class TrackWidget;
class TrackMidi;
class PianoRollContainer;
#include <QWidget>
#include <QPainter>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QMenu>
#include <src/midimanager.h>


class VelocityView : public QGraphicsView{
    Q_OBJECT

public:
    VelocityView(PianoRollContainer* p);
    PianoRollContainer* container(){return _container;}
    void setScale(float x,bool needsReset,int wheelPos);
    void addOrRemoveVelocityViewItem(int start, int velocity,int note, bool adding);
    void changeVelocityViewItemPosition(int oldPos,int newPos, int oldNote,int newNote);
    void populateVelocityViewFromTrack(TrackWidget *trackView);
    void onPianoRollResized(float x);
    void restoreTrack(TrackMidi* midiTrack);
    MidiData *getMidiData();
protected:


private:
    TrackWidget *track_view;
    TrackMidi* _midiTrack;
    PianoRollContainer* _container;

};

#endif // VELOCITYVIEW_H
