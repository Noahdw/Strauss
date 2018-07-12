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

class PianoRollContainer : public QWidget
{
    Q_OBJECT
public:
    PianoRollContainer();
    void propogateFolderViewDoubleClicked(QString filepath,QString path);
    PianoRoll *getPianoRollRef();
    PianoRoll *pianoRoll;
    Keyboard *keyboard;
    ControlChangeContainer *ccContainer;
     QStackedLayout *stackedLayout;
public slots:
   void switchPianoRoll(int id);
   void addPianoRolls(TrackView *view);

private:
    QHBoxLayout *layout;

};

#endif // PIANOROLLCONTAINER_H
