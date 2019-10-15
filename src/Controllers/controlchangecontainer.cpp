#include "controlchangecontainer.h"
#include "pianorollcontainer.h"
#include "src/mastertrack.h"
#include "src/controlchangeitem.h"
#include "src/mastertrack.h"
#include "src/trackmidi.h"
/*
This class holds two views which can be switched between. // TODO, this should be handled by another container
The first is the PianoRollContainer which is a collection of
PianoRolls in a stackedLayout. The second is a StackedWidget of ControlChanges
*/
ControlChangeContainer::ControlChangeContainer(MasterTrack *masterTrack,PianoRollContainer* pianoRollContainer) : _masterTrack(masterTrack)
{
    QVBoxLayout *layout = new QVBoxLayout;
    controlChange = new ControlChange(this);
    layout->addWidget(controlChange);
    setLayout(layout);
}



void ControlChangeContainer::addControlChange()
{
    // auto* cc = new ControlChange(_masterTrack->currentTrack());

}

void ControlChangeContainer::switchControlChange(int index)
{
    currentIndex = index;
    controlChange->switchOverlay(index,_masterTrack->currentTrack());
    //    sLayout->setCurrentIndex(1);
//    (sLayout2->addWidget(_masterTrack->currentTrack()->_controlChange));
//    sLayout2->setCurrentWidget(_masterTrack->currentTrack()->_controlChange);
//    _masterTrack->currentTrack()->_controlChange->switchOverlay(currentIndex);

}
void ControlChangeContainer::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier)
    {
        QWidget::keyPressEvent(event);
        return;
    }
    switch (event->key())
    {
    case Qt::Key_C:
       // switchControlChangeContainer();
        break;

    default:
        break;
    }
    QWidget::keyPressEvent(event);
}

