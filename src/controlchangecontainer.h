#ifndef CONTROLCHANGECONTAINER_H
#define CONTROLCHANGECONTAINER_H

class PianoRollContainer;
class ControlChangeBridge;
class MasterTrack;
#include <QObject>
#include <QWidget>
#include <src/controlchange.h>
#include <QVBoxLayout>
#include <QStackedLayout>

#include <QKeyEvent>

class ControlChangeContainer: public QFrame
{
    Q_OBJECT
public:
    ControlChangeContainer(MasterTrack* masterTrack,PianoRollContainer* pianoRollContainer);
    QStackedLayout *sLayout;
    QStackedLayout *sLayout2;
    QWidget * ccStackedHolder;
    void switchControlChangeContainer();
    void addControlChange();
    void switchControlChange(int index);
    int currentIndex = 0;
private:
    MasterTrack* _masterTrack;
   // PianoRollContainer* _pianoRollContainer;
    ControlChange *lastControlChange;
protected:
    void keyPressEvent(QKeyEvent * event);
};

#endif // CONTROLCHANGECONTAINER_H
