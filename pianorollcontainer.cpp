#include "pianorollcontainer.h"

PianoRollContainer::PianoRollContainer()
{      
    stackedLayout = new QStackedLayout;
    this->setLayout(stackedLayout);
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
    roll->track = view;
    roll->convertTrackToItems();
    Keyboard *key = new Keyboard;
    VelocityView *velocity = new VelocityView;

    QVBoxLayout *vlayout = new QVBoxLayout;
    QHBoxLayout *hlayout = new QHBoxLayout;
    vlayout->addLayout(hlayout,0);
    hlayout->addWidget(key);
    hlayout->addWidget(roll);
    hlayout->setSpacing(0);
    hlayout->setContentsMargins(0,0,0,0);
    vlayout->setSpacing(0);

    QHBoxLayout *hlayout2 = new QHBoxLayout;
    hlayout2->addWidget(velocity,0,Qt::AlignRight);
    vlayout->addLayout(hlayout2);

    QWidget *initview = new QWidget;
    initview->setLayout(vlayout);
    stackedLayout->addWidget(initview);
    QObject::connect(roll,&PianoRoll::updateScrollWheel,key,&Keyboard::scrollWheelChanged);

    emit connectSignals(roll,key,velocity);
}
