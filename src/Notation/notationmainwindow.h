#ifndef NOTATIONMAINWINDOW_H
#define NOTATIONMAINWINDOW_H

class NotationHeader;

#include <QObject>
#include <QWidget>
#include <src/mastertrack.h>
#include <src/trackmidi.h>
#include <QVBoxLayout>
#include "scoreview.h"
#include "score.h"
#include <src/Notation/n_common.h>
class NotationMainWindow : public QWidget
{
    Q_OBJECT
public:
    NotationMainWindow(MasterTrack * master);
    void addInstrument(TrackMidi *track);
    void addMeasure();
private:

    QVBoxLayout *mainVLayout;
    MasterTrack *masterTrack;
    ScoreView *scoreView;
    Score *score;
    NotationHeader *header;
};

#endif // NOTATIONMAINWINDOW_H
