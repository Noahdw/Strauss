#include "controlchangeview.h"
#include <QApplication>
ControlChangeView::ControlChangeView(QWidget *parent) : QGraphicsView(parent)
{
    setViewportUpdateMode(MinimalViewportUpdate);
}


void ControlChangeView::resizeEvent(QResizeEvent *event)
{
    fitInView(scene->sceneRect(),Qt::IgnoreAspectRatio);
}

void ControlChangeView::showEvent(QShowEvent *event)
{
    if (scene == nullptr) return;

    setUpdatesEnabled(false);
    fitInView(scene->sceneRect(), Qt::IgnoreAspectRatio);
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    fitInView(scene->sceneRect(), Qt::IgnoreAspectRatio);
    setUpdatesEnabled(true);
    QGraphicsView::showEvent(event);
}

