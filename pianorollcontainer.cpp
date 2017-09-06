#include "pianorollcontainer.h"

PianoRollContainer::PianoRollContainer()
{    
    pianoRoll = new PianoRoll;
    keyboard = new Keyboard;
    layout = new QHBoxLayout;
    stackedLayout = new QStackedLayout;

    layout->addWidget(keyboard);
    layout->addWidget(pianoRoll);
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);

    QWidget *initview = new QWidget;
    initview->setLayout(layout);
    stackedLayout->addWidget(initview);
    this->setLayout(stackedLayout);

    QObject::connect(pianoRoll,&PianoRoll::updateScrollWheel,keyboard,&Keyboard::scrollWheelChanged);
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

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(key);
    hlayout->addWidget(roll);
    hlayout->setSpacing(0);
    hlayout->setContentsMargins(0,0,0,0);

    QWidget *initview = new QWidget;
    initview->setLayout(hlayout);
    stackedLayout->addWidget(initview);
    QObject::connect(roll,&PianoRoll::updateScrollWheel,key,&Keyboard::scrollWheelChanged);

    emit connectSignals(roll,key);
}
