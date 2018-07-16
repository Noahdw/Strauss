#include "controlchangebridge.h"
#include "src/controlchangeoverlay.h"
#include "src/controlchangeview.h"

ControlChangeBridge::ControlChangeBridge()
{
  //  setMinimumWidth(1000);
   // setMinimumHeight(200);
    overlays.resize(128);
    sLayout = new QStackedLayout;
    //overlay = new ControlChangeOverlay;
    view = new ControlChangeView;

    //sLayout->addWidget(overlay);
    sLayout->setStackingMode(QStackedLayout::StackAll);
    setLayout(sLayout);
    overlays[0] = new ControlChangeOverlay();
    sLayout->addWidget(overlays[0]);
    sLayout->addWidget(view);
}

void ControlChangeBridge::setCurrentOverlay(int index)
{
       verifyOverlayExists(index);
sLayout->replaceWidget(overlays[currentIndex],overlays[index]);
  //  sLayout->removeWidget(overlays[currentIndex]);
    overlays[currentIndex]->hide();
    overlays[index]->show();
  //  sLayout->insertWidget(0,overlays[index]);
    currentIndex = index;
}

void ControlChangeBridge::verifyOverlayExists(int value)
{
    if (overlays[value] == NULL)
    {
        overlays[value] = new ControlChangeOverlay();
        auto scene = new QGraphicsScene;
        scene->setSceneRect(baseScene->sceneRect());
        overlays[value]->scene = scene;
        overlays[value]->setScene(scene);
    }
}
