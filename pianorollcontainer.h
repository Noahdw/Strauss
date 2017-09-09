#ifndef PIANOROLLCONTAINER_H
#define PIANOROLLCONTAINER_H

#include <QObject>
#include <QWidget>
#include <keyboard.h>
#include <pianoroll.h>
#include <QHBoxLayout>
#include <midimanager.h>
#include <QStackedLayout>
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
signals:
   void connectSignals(PianoRoll *proll,Keyboard *key);
private:
    QHBoxLayout *layout;
    QStackedLayout *stackedLayout;
};

#endif // PIANOROLLCONTAINER_H
