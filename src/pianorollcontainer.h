#ifndef PIANOROLLCONTAINER_H
#define PIANOROLLCONTAINER_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <src/midimanager.h>
#include <src/trackwidget.h>

#include <QPainter>

class PianoRoll;
class Keyboard;
class VelocityView;
class TrackLengthView;
class TrackMidi;

class PianoRollContainer : public QFrame
{
    Q_OBJECT
public:
    PianoRollContainer();
    void restoreTrack(TrackMidi *midiTrack);
    VelocityView* velocityView() {return _velocityView;}
    TrackLengthView* trackLengthView(){return _trackLengthView;}
    PianoRoll* pianoRoll(){return _pianoRoll;}
    Keyboard* keyboard(){return _keyboard;}
private:
    QHBoxLayout*  hLayout;
    QVBoxLayout*  mainLayout;
    PianoRoll*    _pianoRoll;
    Keyboard*     _keyboard;
    VelocityView* _velocityView;
    TrackLengthView* _trackLengthView;
};

#endif // PIANOROLLCONTAINER_H
