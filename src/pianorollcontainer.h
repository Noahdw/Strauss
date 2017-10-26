#ifndef PIANOROLLCONTAINER_H
#define PIANOROLLCONTAINER_H

#include <QObject>
#include <QWidget>
#include <keyboard.h>
#include <pianoroll.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <midimanager.h>
#include <trackview.h>
#include <QStackedLayout>
#include <velocityview.h>
class PianoRollContainer : public QWidget
{
    Q_OBJECT
public:
    PianoRollContainer();
    PianoRoll *pianoRoll;
    Keyboard *keyboard;
public slots:
   void switchPianoRoll(int id);
   void addPianoRolls(TrackView *view);
private:
    QHBoxLayout *layout;
    QStackedLayout *stackedLayout;
};

#endif // PIANOROLLCONTAINER_H
