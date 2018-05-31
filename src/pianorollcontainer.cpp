#include "pianorollcontainer.h"

/*This class represents a collection of widgets whose uses are all linked to
 * one another. A Piano roll for editing MIDI, a Keyboard to for simple user
 * playback, and a Velocity view for editing velocities.
 *
 * */

PianoRollContainer::PianoRollContainer()
{      
    stackedLayout = new QStackedLayout;
    this->setLayout(stackedLayout);

}

void PianoRollContainer::propogateFolderViewDoubleClicked(QString filepath, QString path)
{
  PianoRoll*roll = dynamic_cast<PianoRoll*>(stackedLayout->currentWidget()->children().at(3));
  roll->track->folderViewItemDoubleClicked(path + filepath,filepath);
}

void PianoRollContainer::switchPianoRoll(int id)
{
    if (stackedLayout->currentIndex() != id) {
        stackedLayout->setCurrentIndex(id);
    }
}

void PianoRollContainer::addPianoRolls(TrackView *view)
{
    PianoRoll *roll = new PianoRoll;
    Keyboard *key = new Keyboard;
    VelocityView *velocity = new VelocityView;
    TrackLengthView *trackLength = new TrackLengthView;

    roll->track = view;
    if (view->track->totalDT != 0) {

        roll->convertTrackToItems();
    }

    QVBoxLayout *vlayout = new QVBoxLayout;
    QHBoxLayout *hlayout = new QHBoxLayout;
    QHBoxLayout *hlayout2 = new QHBoxLayout;
    QHBoxLayout *hlayout3 = new QHBoxLayout;
    hlayout3->addSpacing(key->width());
    hlayout3->addWidget(trackLength);

    vlayout->addLayout(hlayout3);
    vlayout->addLayout(hlayout,0);

    hlayout->addWidget(key);
    hlayout->addWidget(roll);
    hlayout->setSpacing(0);
    hlayout->setContentsMargins(0,0,0,0);
    vlayout->setSpacing(0);


    hlayout2->addSpacing(key->width());

    hlayout2->addWidget(velocity);
    vlayout->addLayout(hlayout2);


    QWidget *initview = new QWidget;
    initview->setLayout(vlayout);
    stackedLayout->addWidget(initview);

    roll->setKeyboard(key);
    roll->setVelocityView(velocity);
    key->setPianoRoll(roll);
    view->plugin.host->setPianoRollRef(roll);
    velocity->setSceneRect(0,0,roll->totalDT,velocity->height());
    velocity->updateTrackOfItems(view);
    velocity->trackView = view;

}
