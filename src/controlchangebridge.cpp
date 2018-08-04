#include "controlchangebridge.h"
#include "src/controlchangeoverlay.h"
#include "src/controlchangeview.h"

ControlChangeBridge::ControlChangeBridge()
{
    overlays.resize(128);
    sLayout = new QStackedLayout;
    view = new ControlChangeView;

    sLayout->setStackingMode(QStackedLayout::StackAll);
    setLayout(sLayout);
    overlays[0] = new ControlChangeOverlay(0);
    sLayout->addWidget(overlays[0]);
    sLayout->addWidget(view);
}

void ControlChangeBridge::setCurrentOverlay(int index)
{
    verifyOverlayExists(index);
    sLayout->replaceWidget(overlays[currentIndex],overlays[index]);
    overlays[currentIndex]->hide();
    overlays[index]->show();
    currentIndex = index;
}

void ControlChangeBridge::verifyOverlayExists(int value)
{
    if (overlays[value] == NULL)
    {
        overlays[value] = new ControlChangeOverlay(value);
        auto scene = new QGraphicsScene;
        scene->setSceneRect(baseScene->sceneRect());
        overlays[value]->updateScene(scene);
    }
}
