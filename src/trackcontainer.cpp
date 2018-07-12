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
    vSplitter = new QSplitter(this);
    vSplitter->setOrientation(Qt::Vertical);
    vSplitter->setContentsMargins(0,0,0,0);
    vSplitter->setHandleWidth(0);
    vLayout = new QVBoxLayout;
    vLayout->setSpacing(0);
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    hLayout = new QHBoxLayout;
    hLayout->setSpacing(0);
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->addLayout(vLayout);
    auto lt = new QVBoxLayout;
    lt->setSpacing(0);
    lt->setContentsMargins(0,0,0,0);
    lt->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    lt->addWidget(vSplitter);
    hLayout->addLayout(lt);
    setLayout(hLayout);
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

    view->id = ID;
    ID++;
    auto *midiView = new TrackMidiView;
    view->trackMidiView = midiView;

    emit addPianoRoll(view);
    QObject::connect(view,&TrackView::trackClickedOn,prcontainer,&PianoRollContainer::switchPianoRoll);

    vLayout->addWidget(midiView);
    vSplitter->addWidget(view);

}
void TrackContainer::mousePressEvent(QMouseEvent *event)
{

}

void TrackContainer::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_C:
        switchControlChange();
        break;
    default:
        break;
    }
 QWidget::keyPressEvent(event);
}

