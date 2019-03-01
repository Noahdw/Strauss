#include "notationmainwindow.h"
#include "src/Notation/notationheader.h"


NotationMainWindow::NotationMainWindow(MasterTrack *master) : masterTrack(master)
{
    score = new Score;
    scoreView = new ScoreView(score);
    mainVLayout = new QVBoxLayout;
    header = new NotationHeader(this);
    mainVLayout->addWidget(header);
    mainVLayout->addWidget(scoreView);
    setLayout(mainVLayout);

    score->addStave(0);
    for(int i = 0; i < 10; i++)
        score->addMeasure(i);

}

void NotationMainWindow::addInstrument(TrackMidi *track)
{

}

void NotationMainWindow::addMeasure()
{

}

