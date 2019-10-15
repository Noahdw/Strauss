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
    MasterTrack* _masterTrack;
    QWidget * ccStackedHolder;
    void addControlChange();
    void switchControlChange(int index);
    int currentIndex = 0;
private:

    ControlChange *controlChange;
protected:
    void keyPressEvent(QKeyEvent * event);
};

#endif // CONTROLCHANGECONTAINER_H
