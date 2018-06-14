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
    //  setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    setLayout(vLayout);
}

void TrackContainer::addTrackView(mSong *song)
{

    foreach (const auto &track, song->tracks) {
        QWidget *widget = new QWidget;
        auto *view = new TrackView(track);
        auto *hlayout = new QHBoxLayout;
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

        auto *midiView = new TrackMidiView;
        view->trackMidiView = midiView;
        hlayout->addWidget(midiView);
        vLayout->addWidget(widget,Qt::AlignTop|Qt::AlignLeft);
       // vLayout->setAlignment(widget,Qt::AlignTop);
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
    auto *hlayout = new QHBoxLayout;
    hlayout->setAlignment(Qt::AlignTop);
    hlayout->setSpacing(0);
    hlayout->setContentsMargins(0,0,0,0);
    widget->setLayout(hlayout);
    //hlayout->addWidget(view); for left placing


    view->id = ID;
    ID++;
    auto *midiView = new TrackMidiView;
    view->trackMidiView = midiView;

    emit addPianoRoll(view);
    QObject::connect(view,&TrackView::trackClickedOn,prcontainer,&PianoRollContainer::switchPianoRoll);
    adjustSize();

    hlayout->addWidget(midiView);
    hlayout->addWidget(view);

    vLayout->addWidget(widget,Qt::AlignTop|Qt::AlignLeft);
   // QSpacerItem *verticalSpacer = new QSpacerItem(1, 200, QSizePolicy::Minimum, QSizePolicy::Expanding);
  //  if(ID == 1)

 //vLayout->addItem(verticalSpacer);
   // vLayout->insertSpacerItem(-1,verticalSpacer);
    adjustSize();


}
void TrackContainer::mousePressEvent(QMouseEvent *event)
{

}

