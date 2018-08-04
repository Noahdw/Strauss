#include "controlchangecontainer.h"
#include "src/pianorollcontainer.h"
#include "src/controlchangebridge.h"
#include "src/controlchangeitem.h"
/*
This class holds two views which can be switched between.
The first is the PianoRollContainer which is a collection of
PianoRolls in a stackedLayout. The second is a ControlChangeBridge
which is simply a container for ccViews and ccLayouts. It is placed into a stacked layout
in this class and is linked to the pianoRolls as they are both dependant on eachother.
*/
ControlChangeContainer::ControlChangeContainer(PianoRollContainer *pianoRollContainer)
{
    piano_roll_container = pianoRollContainer;
    sLayout = new QStackedLayout;
    sLayout2 = new QStackedLayout;
    ccStackedHolder = new QWidget;

    ccStackedHolder->setLayout(sLayout2);
    sLayout->addWidget(pianoRollContainer);
    sLayout->addWidget(ccStackedHolder);
    setLayout(sLayout);
}

void ControlChangeContainer::switchControlChangeContainer()
{
    sLayout->setCurrentIndex(!sLayout->currentIndex());
    sLayout2->setCurrentIndex(piano_roll_container->stackedLayout->currentIndex());
}

void ControlChangeContainer::addControlChangeView(PianoRoll * pianoRoll)
{
    ControlChangeBridge * bridge = new ControlChangeBridge;
    QGraphicsScene * scene = new QGraphicsScene;
    bridge->view->pRoll = pianoRoll;
    pianoRoll->bridge = bridge;
    bridge->view->scene = pianoRoll->scene;
    bridge->view->setScene(pianoRoll->scene);
    bridge->baseScene = scene;
    bridge->overlays[bridge->currentIndex]->updateScene(scene);
    scene->setSceneRect(*pianoRoll->sceneRect);

    sLayout2->addWidget(bridge);
    sLayout2->setCurrentIndex(0);
}

void ControlChangeContainer::switchControlChangeType(int index)
{
   auto w = dynamic_cast<ControlChangeBridge*>(sLayout2->currentWidget());
   w->setCurrentOverlay(index);
}
void ControlChangeContainer::keyPressEvent(QKeyEvent *event)
{
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

