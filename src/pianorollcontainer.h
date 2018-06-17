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

class PianoRollContainer : public QWidget
{
    Q_OBJECT
public:
    PianoRollContainer();
    void propogateFolderViewDoubleClicked(QString filepath,QString path);

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
