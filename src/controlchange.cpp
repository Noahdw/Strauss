#include "controlchange.h"
#include <QApplication>
#include "trackmidi.h"
#include "controlchangeoverlay.h"
#include "pianoroll.h"
ControlChange::ControlChange(TrackMidi *track) : _midiTrack(track)
{
    backgroundView = new QGraphicsView;
    //  backgroundView->setInteractive(false);
    backgroundView->installEventFilter(this);

    stackedLayout = new QStackedLayout;
    stackedLayout->setStackingMode(QStackedLayout::StackAll);
    stackedLayout->addWidget(backgroundView);
    setLayout(stackedLayout);
    backgroundView->setScene(track->midiEditorState()->pianoRollScene);
}

void ControlChange::addOverlay(int index)
{
    if(_midiTrack->addCC(index))
    {
        _overlays[index] = std::make_unique<ControlChangeOverlay>(index,this);
    }
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

void ControlChange::switchOverlay(int index)
{
    if(_overlays.count(index) == 0)
    {
        addOverlay(index);
    }
    if(lastOverlay == nullptr)
        lastOverlay = _overlays[index].get();
    stackedLayout->addWidget(_overlays[index].get());
    stackedLayout->setCurrentWidget(_overlays[index].get());
    if(lastOverlay != _overlays[index].get())
        stackedLayout->removeWidget(lastOverlay);
   // _overlays[index].get()->show();
    lastOverlay = _overlays[index].get();
    currentIndex = index;
    backgroundView->fitInView(backgroundView->sceneRect());
    lastOverlay->updateScene(backgroundView->scene()->sceneRect());

}

void ControlChange::showEvent(QShowEvent *event)
{

}

