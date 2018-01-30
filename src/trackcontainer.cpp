#include "trackcontainer.h"
#include <QWidget>
#include <src/trackmidiview.h>
int ID = 0;
/*This class represents a container for the tracks, located above the Pianoroll.
 *
 *
 **/
TrackContainer::TrackContainer()
{
vLayout = new QVBoxLayout;
vLayout->setSpacing(0);
vLayout->setContentsMargins(0,0,0,0);
vLayout->setAlignment(Qt::AlignTop|Qt::AlignLeft);
setLayout(vLayout);
}

void TrackContainer::addTrackView(mSong *song)
{

    foreach (auto track, song->tracks) {
        QWidget *widget = new QWidget;
        TrackView *view = new TrackView(track);
        QHBoxLayout *hlayout = new QHBoxLayout;
        hlayout->setAlignment(Qt::AlignTop);
        hlayout->setSpacing(0);
        hlayout->setContentsMargins(0,0,0,0);
        widget->setLayout(hlayout);
        hlayout->addWidget(view);


       // vLayout->addWidget(view,Qt::AlignTop|Qt::AlignLeft);
        view->id = ID;
        emit addPianoRoll(view);
        ID++;

        QObject::connect(view,&TrackView::trackClickedOn,prcontainer,&PianoRollContainer::switchPianoRoll);

        TrackMidiView *midiView = new TrackMidiView;
        view->trackMidiView = midiView;
        hlayout->addWidget(midiView);
        vLayout->addWidget(widget,Qt::AlignTop|Qt::AlignLeft);
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

    QWidget *widget = new QWidget;
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setAlignment(Qt::AlignTop);
    hlayout->setSpacing(0);
    hlayout->setContentsMargins(0,0,0,0);
    widget->setLayout(hlayout);
    hlayout->addWidget(view);


     view->id = ID;
     ID++;
     emit addPianoRoll(view);
     QObject::connect(view,&TrackView::trackClickedOn,prcontainer,&PianoRollContainer::switchPianoRoll);
     adjustSize();
     TrackMidiView *midiView = new TrackMidiView;
     view->trackMidiView = midiView;

     hlayout->addWidget(midiView);
     vLayout->addWidget(widget,Qt::AlignTop|Qt::AlignLeft);
     adjustSize();


}
void TrackContainer::mousePressEvent(QMouseEvent *event)
{

}

