#include "trackcontainer.h"

TrackContainer::TrackContainer()
{
vLayout = new QVBoxLayout;
vLayout->setSpacing(0);
vLayout->setContentsMargins(0,0,0,0);

setLayout(vLayout);
}

void TrackContainer::addTrackView(mSong *song)
{
    int i = 0;
    foreach (auto track, song->tracks) {
        TrackView *view = new TrackView(song->tracks.at(i));
        vLayout->addWidget(view,Qt::AlignTop|Qt::AlignLeft);
        view->id = i;
        emit addPianoRoll(view);
        i++;
         QObject::connect(view,&TrackView::trackClickedOn,prcontainer,&PianoRollContainer::switchPianoRoll);
    }
    adjustSize();
}

void TrackContainer::setPianoRollReference(PianoRollContainer *prc)
{
    prcontainer = prc;
     QObject::connect(this,&TrackContainer::addPianoRoll,prcontainer,&PianoRollContainer::addPianoRolls);

}
void TrackContainer::mousePressEvent(QMouseEvent *event)
{

}

