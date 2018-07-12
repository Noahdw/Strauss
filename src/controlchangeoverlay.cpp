#include "controlchangeoverlay.h"
#include "src/controlchangeitem.h"
#include "src/collisionitem.h"

ControlChangeOverlay::ControlChangeOverlay(QWidget *parent) : QGraphicsView(parent)
{
    setMinimumWidth(1000);
    setMinimumHeight(200);
    setStyleSheet("background-color: transparent;");
    setViewportUpdateMode(FullViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    leftItem      = new ControlChangeItem;
    rightItem     = new ControlChangeItem;
   // rectItem      = new QGraphicsRectItem(0,0,500,2000);
    collisionItem = new CollisionItem(this);
    rubberBand    = new QRubberBand(QRubberBand::Rectangle, this);
  //  rectItem->hide();
}

void ControlChangeOverlay::createLineConnector()
{
    int size = lineItems.size();

    for (int i = 0; i < size; ++i)
    {
        scene->removeItem(lineItems[i]);
        delete lineItems.at(i);
    }
    QList<QGraphicsItem*> tempItem = scene->items(Qt::AscendingOrder);
    lineItems.clear();

    size = tempItem.size() - 1;

    QMap<int, QGraphicsItem*>::const_iterator i = activeItems.constBegin();
    while (i != activeItems.constEnd()) {
        auto item = *i;
        ++i;
        if (i != activeItems.constEnd())
        {
            auto item2 = *i;
            QPen pen(Qt::red,4);
            lineItems.push_back(scene->addLine(item->x(),item->y(),item2->x(),item2->y(),pen));
        }
    }
}

void ControlChangeOverlay::removeSelectedItems()
{
    for(const auto& var : scene->selectedItems())
    {
         auto item = dynamic_cast<ControlChangeItem*>(var);
         if (item && item != leftItem && item != rightItem)
         {
             scene->removeItem(var);
            activeItems.remove(item->x());
             delete item;
             item=nullptr;
         }

    }
    scene->selectedItems().clear();
    createLineConnector();
}

void ControlChangeOverlay::removeCollidingItems(QList<QGraphicsItem *> &items)
{
    for(const auto &i : items)
    {
        auto ccItem = dynamic_cast<ControlChangeItem*>(i);
        if (ccItem && ccItem != leftItem && ccItem != rightItem)
        {
            scene->removeItem(ccItem);
            activeItems.remove(ccItem->x());
            delete ccItem;
        }

    }
}

void ControlChangeOverlay::setCurrentOverlay(int index)
{

}

bool ControlChangeOverlay::eventFilter(QObject *target, QEvent *event)
{

    if(event->type() == QEvent::GraphicsSceneMousePress )
    {
        auto mouseEvent = dynamic_cast<QGraphicsSceneEvent*>(event);
        if (mouseEvent)
        {

            auto me = dynamic_cast<QGraphicsSceneMouseEvent*>(mouseEvent);
            if (me)
            {
                 qDebug() << me->pos().x();
                 collisionItem->setPos(me->pos().x(),0);
            }

        }

    }

    return false;
    // return TrackView::eventFilter(target,event);
}

void ControlChangeOverlay::showEvent(QShowEvent *event)
{
    if (scene == nullptr)
    {
        return;
    }
    if (firstShow)
    {
        scene->addItem(leftItem);
        scene->addItem(rightItem);
        scene->addItem(collisionItem);
        collisionItem->setZValue(2);
        leftItem->setPos(0,1092);
        rightItem->setPos(960*60,1092);
        activeItems.insert(0,leftItem);
        activeItems.insert(960*60,rightItem);
        createLineConnector();
        firstShow  = false;
        leftItem->overlay = this;
        rightItem->overlay = this;
    }
    //For some reason fitInView needs to be called twice manually to work,
    //this is some weird workaround.
    setUpdatesEnabled(false);
    fitInView(scene->sceneRect(), Qt::IgnoreAspectRatio);
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    fitInView(scene->sceneRect(), Qt::IgnoreAspectRatio);
    setUpdatesEnabled(true);
    // createLineConnector();
    QGraphicsView::showEvent(event);

}

void ControlChangeOverlay::mousePressEvent(QMouseEvent *event)
{
    QGraphicsItem *item = itemAt(event->pos());
    origin = event->pos();

   // rectItem->setPos(origin);
    if (!item && !canDraw)
    {

        setDragMode(QGraphicsView::RubberBandDrag);
        rubberBand->setGeometry(QRect(origin,QSize()));
        rubberBand->show();
    }
    else if(canDraw)
    {
         collisionItem->setPos(mapToScene(origin).x(),0);
    }
    QGraphicsView::mousePressEvent(event);
}

void ControlChangeOverlay::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug() << "OVERLAY";
    auto t = mapToScene(event->pos());
    ControlChangeItem *item = new ControlChangeItem();
    item->setPos(t);
    item->overlay = this;
    activeItems.insert(t.x(),item);
    scene->addItem(item);
    createLineConnector();
    QGraphicsView::mouseDoubleClickEvent(event);
}

void ControlChangeOverlay::mouseMoveEvent(QMouseEvent *event)
{

    if (canDraw)
    {

        auto newPos = mapToScene(event->pos());

      //  rectItem->setPos(newPos.x(),0);
           qDebug() << newPos.x();
        //   rectItem->setRect(newPos.x(),0,);
//       auto list = scene->collidingItems(rectItem);
//       for (const auto& var : list)
//       {
//           auto ccItem = dynamic_cast<ControlChangeItem*>(var);
//           if (ccItem)
//           {
//               scene->removeItem(ccItem);
//               activeItems.remove(ccItem->x());
//               delete ccItem;
//           }

//       }

        ControlChangeItem *item = new ControlChangeItem();
       // item->setPos(newPos);
        item->setInitalPos(newPos);
        item->overlay = this;
        activeItems.insert(newPos.x(),item);
        scene->addItem(item);
        createLineConnector();
    }
    else
    {
         rubberBand->setGeometry(QRect(origin,event->pos()).normalized());
    }
    QGraphicsView::mouseMoveEvent(event);
}

void ControlChangeOverlay::mouseReleaseEvent(QMouseEvent *event)
{
     rubberBand->hide();
     QGraphicsView::mouseReleaseEvent(event);
}

void ControlChangeOverlay::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_B:
        canDraw = !canDraw;
        break;
    case Qt::Key_Delete:
        removeSelectedItems();
        break;
    default:
        break;
    }
    QGraphicsView::keyPressEvent(event);
}
