#include "controlchange.h"
#include <QApplication>
#include "trackmidi.h"
#include "controlchangeoverlay.h"
#include "pianorollwidget.h"
#include "Controllers/controlchangecontainer.h"
#include "mastertrack.h"
ControlChange::ControlChange(ControlChangeContainer *container) : _container(container)
{
    _overlay = new ControlChangeOverlay(this);
    backgroundView = new QGraphicsView;
    backgroundView->installEventFilter(this);

    stackedLayout = new QStackedLayout;
    stackedLayout->setStackingMode(QStackedLayout::StackAll);
    stackedLayout->addWidget(backgroundView);
    stackedLayout->addWidget(_overlay);
    setLayout(stackedLayout);
    //switchOverlay(0,container->_masterTrack->currentTrack());
   // backgroundView->setScene(container->_masterTrack->currentTrack()->midiEditorState()->pianoRollScene);
}

void ControlChange::addOverlay(int index)
{

}
bool ControlChange::eventFilter(QObject *target, QEvent *event)
{
    // qDebug() << event->type();
    if(event->type() == QEvent::GraphicsSceneMousePress || event->type() == QEvent::MouseButtonPress)
    {

    }

    return false;
    // return TrackView::eventFilter(target,event);
}


void ControlChange::resizeEvent(QResizeEvent *event)
{

}

void ControlChange::switchOverlay(int index,TrackMidi *track)
{
    Q_ASSERT(index >= 0 && index < 255);
    _midiTrack = track;
    currentIndex = index;
    auto state = track->midiEditorState();
    backgroundView->setScene(state->pianoRollScene);
    if(state->controlScenes.count(index) == 0)
    {
       state->controlScenes[index] = std::make_unique<QGraphicsScene>();
       //state->controlLists[index] = std::make_unique<std::map<int,int>>();
    }

    //_overlay->restoreOverlay(state->controlScenes.at(index).get(),state->controlLists.at(index).get());


    //    if(_overlays.count(index) == 0)
    //    {
    //        addOverlay(index);
    //    }
    //    if(lastOverlay == nullptr)
    //        lastOverlay = _overlays[index].get();
    //    stackedLayout->addWidget(_overlays[index].get());
    //    stackedLayout->setCurrentWidget(_overlays[index].get());
    //    if(lastOverlay != _overlays[index].get())
    //        stackedLayout->removeWidget(lastOverlay);
    //   // _overlays[index].get()->show();
    //    lastOverlay = _overlays[index].get();
    //    currentIndex = index;
    //    backgroundView->fitInView(backgroundView->sceneRect());
    //    lastOverlay->fitIntoView();
    //    lastOverlay->updateScene(backgroundView->scene()->sceneRect());

}

void ControlChange::showEvent(QShowEvent *event)
{

}

