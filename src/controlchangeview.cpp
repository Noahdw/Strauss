#include "controlchangeview.h"
#include <QApplication>
ControlChangeView::ControlChangeView(QWidget *parent) : QGraphicsView(parent)
{
    // setMinimumWidth(1000);
    // setMinimumHeight(200);
    setStyleSheet("background-color: transparent;");
    setViewportUpdateMode(MinimalViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
}
void ControlChangeView::paintEvent(QPaintEvent *event)
{
    QPainter painter(viewport());
    QPen pen;
    pen.setColor(Qt::black);
    painter.setBrush(Qt::lightGray);
    painter.setPen(pen);
    painter.fillRect(viewport()->rect(),painter.brush());
    QGraphicsView::paintEvent(event);
}

void ControlChangeView::resizeEvent(QResizeEvent *event)
{
    fitInView(scene->sceneRect(),Qt::IgnoreAspectRatio);
}
void ControlChangeView::mousePressEvent(QMouseEvent *event)
{

}

void ControlChangeView::mouseDoubleClickEvent(QMouseEvent *event)
{
}

void ControlChangeView::showEvent(QShowEvent *event)
{
    if (scene == nullptr)
    {
        return;
    }
    setUpdatesEnabled(false);
    fitInView(scene->sceneRect(), Qt::IgnoreAspectRatio);
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    fitInView(scene->sceneRect(), Qt::IgnoreAspectRatio);
    setUpdatesEnabled(true);
    QGraphicsView::showEvent(event);
}

