#include "controlchangecontainer.h"
#include "src/pianorollcontainer.h"
#include "mastertrack.h"
#include "src/controlchangeitem.h"
#include "mastertrack.h"
#include "trackmidi.h"
/*
This class holds two views which can be switched between. // TODO, this should be handled by another container
The first is the PianoRollContainer which is a collection of
PianoRolls in a stackedLayout. The second is a StackedWidget of ControlChanges
*/
ControlChangeContainer::ControlChangeContainer(MasterTrack *masterTrack,PianoRollContainer* pianoRollContainer) : _masterTrack(masterTrack), _pianoRollContainer(pianoRollContainer)
{
    sLayout = new QStackedLayout;
    sLayout2 = new QStackedLayout;
    ccStackedHolder = new QWidget;

    ccStackedHolder->setLayout(sLayout2);
    sLayout->addWidget(_pianoRollContainer);
    sLayout->addWidget(ccStackedHolder);
    setLayout(sLayout);
}

void ControlChangeContainer::switchControlChangeContainer()
{
//    if(ccView)
//    {
//        sLayout->setCurrentIndex(1);
//        sLayout2->setCurrentIndex(_pianoRollContainer->stackedLayout->currentIndex());
//    }
  //  else {
        sLayout->setCurrentIndex(!sLayout->currentIndex());
        sLayout2->setCurrentIndex(_pianoRollContainer->stackedLayout->currentIndex());
        _masterTrack->currentTrack()->_controlChange->switchOverlay(currentIndex);
 //   }

}

void ControlChangeContainer::addControlChange()
{
   // auto* cc = new ControlChange(_masterTrack->currentTrack());

}

void ControlChangeContainer::switchControlChange(int index)
{
    currentIndex = index;
      sLayout->setCurrentIndex(1);
    (sLayout2->addWidget(_masterTrack->currentTrack()->_controlChange));
    sLayout2->setCurrentWidget(_masterTrack->currentTrack()->_controlChange);
    _masterTrack->currentTrack()->_controlChange->switchOverlay(currentIndex);

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
        switchControlChangeContainer();
        break;

    default:
        break;
    }
        QWidget::keyPressEvent(event);
}

