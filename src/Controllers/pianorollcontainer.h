#ifndef PIANOROLLCONTAINER_H
#define PIANOROLLCONTAINER_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <src/midimanager.h>
#include <src/trackwidget.h>

#include <QPainter>

class PianoRollWidget;
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
    void playKeyboardNote(int note, bool active);
    void scaleWidgets(QMatrix matrix);
    void setScrollPositions(int value, Qt::Orientation axis);
    void setScaleFactor(qreal scale);
    VelocityView* velocityView() {return _velocityView;}
    TrackLengthView* trackLengthView(){return _trackLengthView;}
    PianoRollWidget* pianoRoll(){return _pianoRoll;}
    Keyboard* keyboard(){return _keyboard;}
private:
    QHBoxLayout*  hLayout;
    QVBoxLayout*  mainLayout;
    PianoRollWidget*    _pianoRoll;
    Keyboard*     _keyboard;
    VelocityView* _velocityView;
    TrackMidi*    _currentMidiTrack;
    TrackLengthView* _trackLengthView;
};

#endif // PIANOROLLCONTAINER_H
