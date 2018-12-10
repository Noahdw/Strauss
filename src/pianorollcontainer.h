#ifndef PIANOROLLCONTAINER_H
#define PIANOROLLCONTAINER_H

#include <QObject>
#include <QWidget>
#include <src/keyboard.h>
#include <src/pianoroll.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <src/midimanager.h>
#include <src/trackview.h>
#include <QStackedLayout>
#include <src/velocityview.h>
#include <src/tracklengthview.h>
#include <src/controlchangecontainer.h>
#include <QPainter>

class PianoRollContainer : public QFrame
{
    Q_OBJECT
public:
    PianoRollContainer();
    void setControlChangeContainer(ControlChangeContainer *_controlChangeContainer); // must be called before class can be used
    void deletePianoRoll();
    PianoRoll *getCurrentPianoRoll();
    QStackedLayout *stackedLayout;
public slots:
    void switchPianoRoll(TrackView *track_view);
    PianoRoll *addPianoRoll(TrackView *trackView);
protected:
private:
    QHBoxLayout *layout;
    ControlChangeContainer *controlChangeContainer;

};

#endif // PIANOROLLCONTAINER_H
