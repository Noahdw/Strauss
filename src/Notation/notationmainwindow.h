#ifndef NOTATIONMAINWINDOW_H
#define NOTATIONMAINWINDOW_H

class NotationHeader;

#include <QObject>
#include <QWidget>
#include <src/mastertrack.h>
#include <src/trackmidi.h>
#include <src/notation/notationview.h>
#include <QVBoxLayout>
#include <src/Notation/n_common.h>
class NotationMainWindow : public QWidget
{
    Q_OBJECT
public:
    NotationMainWindow(MasterTrack * master);
    void addInstrument(TrackMidi *track);
    void addMeasure();
    NotationView *notationView;
private:

    QVBoxLayout *mainVLayout;
    MasterTrack *masterTrack;
    NotationHeader *header;
};

#endif // NOTATIONMAINWINDOW_H
