#include "controlchangebridge.h"
#include "src/controlchangeoverlay.h"
#include "src/controlchangeview.h"

ControlChangeBridge::ControlChangeBridge()
{
    setMinimumWidth(1000);
    setMinimumHeight(200);
    sLayout = new QStackedLayout;
    overlay = new ControlChangeOverlay;
    view = new ControlChangeView;
    sLayout->addWidget(view);
    sLayout->addWidget(overlay);
    sLayout->setStackingMode(QStackedLayout::StackAll);
    setLayout(sLayout);
   // sLayout->setCurrentIndex(1);
}
