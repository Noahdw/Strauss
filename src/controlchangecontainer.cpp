#include "controlchangecontainer.h"
#include "src/pianorollcontainer.h"
#include "src/controlchangebridge.h".h"

/*
This class holds two views which can be switched between.
The first is the PianoRollContainer which is a collection of
PianoRolls in a stackedLayout. The second is a ControlChangeBridge
which is simply a container for ccViews and ccLayouts. It is placed into a stacked layout
in this class and is linked to the pianoRolls as they are both dependant on eachother.
*/
ControlChangeContainer::ControlChangeContainer(PianoRollContainer *pRollCont)
{
    sLayout = new QStackedLayout;
    sLayout2 = new QStackedLayout;
    ccStackedHolder = new QWidget;
    ccStackedHolder->setMinimumWidth(1000);
    ccStackedHolder->setMinimumHeight(200);
    pRollCont->ccContainer = this;
    ccStackedHolder->setLayout(sLayout2);
    sLayout->addWidget(pRollCont);
    sLayout->addWidget(ccStackedHolder);
    pianoRollCont = pRollCont;
    setLayout(sLayout);
}

void ControlChangeContainer::switchControlChangeContainer()
{
    sLayout->setCurrentIndex(!sLayout->currentIndex());
    sLayout2->setCurrentIndex(pianoRollCont->stackedLayout->currentIndex());
}

void ControlChangeContainer::addControlChangeView(PianoRoll * pianoRoll)
{
    ControlChangeBridge * bridge = new ControlChangeBridge;
    QGraphicsScene * scene = new QGraphicsScene;
    bridge->view->pRoll = pianoRoll;
    bridge->view->scene = pianoRoll->scene;
    bridge->view->setScene(pianoRoll->scene);

 //   scene->set
    bridge->overlay->scene = scene;
    scene->setSceneRect(*pianoRoll->sceneRect);
    bridge->overlay->setScene(scene);
    sLayout2->addWidget(bridge);
    sLayout2->setCurrentIndex(0);
}

void ControlChangeContainer::switchControlChangeType(int index)
{
   auto w = dynamic_cast<ControlChangeBridge*>(sLayout2->currentWidget());
   w->overlay->setCurrentOverlay(index);
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

