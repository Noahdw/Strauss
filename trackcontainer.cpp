#include "trackcontainer.h"

int ID = 0;

TrackContainer::TrackContainer()
{
vLayout = new QVBoxLayout;
vLayout->setSpacing(0);
vLayout->setContentsMargins(0,0,0,0);

setLayout(vLayout);
}

void TrackContainer::addTrackView(mSong *song)
{

    foreach (auto track, song->tracks) {
        TrackView *view = new TrackView(track);
        vLayout->addWidget(view,Qt::AlignTop|Qt::AlignLeft);
        view->id = ID;
        emit addPianoRoll(view);
        ID++;

        QObject::connect(view,&TrackView::trackClickedOn,prcontainer,&PianoRollContainer::switchPianoRoll);
    }
    adjustSize();
}

void TrackContainer::setPianoRollReference(PianoRollContainer *prc)
{
    prcontainer = prc;
     QObject::connect(this,&TrackContainer::addPianoRoll,prcontainer,&PianoRollContainer::addPianoRolls);

}

void TrackContainer::addSingleView(TrackView *view)
{
    vLayout->addWidget(view,Qt::AlignTop|Qt::AlignLeft);
     view->id = ID;
     ID++;
     emit addPianoRoll(view);
     QObject::connect(view,&TrackView::trackClickedOn,prcontainer,&PianoRollContainer::switchPianoRoll);
     adjustSize();
}
void TrackContainer::mousePressEvent(QMouseEvent *event)
{

}

