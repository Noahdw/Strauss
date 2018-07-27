#include "controlchangeoverlay.h"
#include "src/controlchangeitem.h"
#include "src/collisionitem.h"

ControlChangeOverlay::ControlChangeOverlay(QWidget *parent) : QGraphicsView(parent)
{
    // setMinimumWidth(1000);
    //  setMinimumHeight(200);
    setStyleSheet("background-color: transparent;");
    setViewportUpdateMode(FullViewportUpdate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHint(QPainter::Antialiasing);
    leftItem      = new ControlChangeItem;
    rightItem     = new ControlChangeItem;
    collisionItem = new CollisionItem(this);
    rubberBand    = new QRubberBand(QRubberBand::Rectangle, this);

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

    std::map<int, QGraphicsItem*>::const_iterator i = activeItems.begin();
    int j = 0;
    while (i != activeItems.end()) {
        auto item = *i;
        ++i;
        if (i != activeItems.end())
        {
            auto item2 = *i;
            QPen pen(Qt::red,4);

            lineItems.push_back(scene->addLine(item.second->x(),item.second->y(),item2.second->x(),item2.second->y(),pen));
            lineItems.at(j++)->hide();
        }
    }
    viewport()->update();
}

// Removes rubberband selected items
void ControlChangeOverlay::removeSelectedItems()
{
    qDebug() << scene->selectedItems().size();
    for(const auto& var : scene->selectedItems())
    {
        auto item = dynamic_cast<ControlChangeItem*>(var);
        if (item && item != leftItem && item != rightItem)
        {
            scene->removeItem(item);
            activeItems.erase(item->x());
            delete item;
        }

    }
    scene->clearSelection();
    createLineConnector();
}

//Called by collisionItem
void ControlChangeOverlay::removeCollidingItems(QList<QGraphicsItem *> &items)
{
    for(const auto &i : items)
    {
        auto ccItem = dynamic_cast<ControlChangeItem*>(i);
        if (ccItem && ccItem != recentItem && ccItem != leftItem && ccItem != rightItem)
        {
            scene->removeItem(ccItem);
            activeItems.erase(ccItem->x());
            delete ccItem;
        }

    }
}

//Adds a value from midi recording / files
void ControlChangeOverlay::addPoint(int x, int value)
{if (!activeItems.count(x))
    {
        value = 127 - value;
        ControlChangeItem *item = new ControlChangeItem();
        item->setX(x);
        int OldRange = (127 - 0);
        int NewRange = (scene->height() - 0);
        int NewValue = (((value - 0) * NewRange) / OldRange) + 0;
        item->setY(NewValue);
        item->overlay = this;
        item->value = 127 - value;
        activeItems[x] = item;

        scene->addItem(item);
        createLineConnector();
    }
}

//For some reason fitInView needs to be called twice manually to work,
//this is some weird workaround.
void ControlChangeOverlay::fitIntoView()
{
    setUpdatesEnabled(false);
    fitInView(scene->sceneRect(), Qt::IgnoreAspectRatio);
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    fitInView(scene->sceneRect(), Qt::IgnoreAspectRatio);
    setUpdatesEnabled(true);
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
        activeItems[0] = leftItem;
        activeItems[960*60] = rightItem;
        createLineConnector();
        firstShow  = false;
        leftItem->overlay = this;
        rightItem->overlay = this;
        leftItem->hide();
        rightItem->hide();
    }
    fitIntoView();

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
    //wtf why no work?
    for(const auto & i : scene->items(mapToScene(event->pos()),Qt::IntersectsItemShape))
    {
        auto cci = dynamic_cast<ControlChangeItem*>(i);
        if (cci)
        {
            scene->removeItem(cci);
            activeItems.erase(cci->x());
            delete cci;
            return;
        }
    }
    //Add new item
    auto t = mapToScene(event->pos());
    ControlChangeItem *item = new ControlChangeItem();
    item->setPos(t);
    item->overlay = this;
    activeItems[t.x()] = item;
    scene->addItem(item);
    createLineConnector();
    recalculateDT();
    QGraphicsView::mouseDoubleClickEvent(event);
}

void ControlChangeOverlay::mouseMoveEvent(QMouseEvent *event)
{
    if (canDraw)
    {
        auto newPos = mapToScene(event->pos());

        ControlChangeItem *item = new ControlChangeItem();
        recentItem = item;
        scene->addItem(item);
        item->setInitalPos(newPos);
        item->overlay = this;
        if (item->x() == 0 && activeItems.count(0))
        {
            scene->removeItem(item);
            delete item;
            return;
        }
        else if(item->x() == scene->width() && activeItems.count(scene->width()))
        {
            scene->removeItem(item);
            delete item;
            return;
        }
        activeItems[item->x()] = item;

        int OldRange = (scene->height() - 0);
        int NewRange = (127 - 1);
        int NewValue =127 - (((item->y() - 0) * NewRange) / OldRange);
        item->value = NewValue;
        if (canDraw)
        {
            item->hide();
        }
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
    if (canDraw)
    {
        recalculateDT();
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void ControlChangeOverlay::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier)
    {
        switch (event->key())
        {
        case Qt::Key_X:
            removeSelectedItems(); // just delete for now
            break;
        default:
            break;
        }
    }
    else
    {
        switch (event->key())
        {
        case Qt::Key_B:
            switchDrawModes();
            break;
        case Qt::Key_Delete:
            removeSelectedItems();
            break;
        default:
            break;
        }
    }
    QGraphicsView::keyPressEvent(event);
}

void ControlChangeOverlay::resizeEvent(QResizeEvent *event)
{
    if (!firstShow)
    {
        fitIntoView();
    }
    QGraphicsView::resizeEvent(event);
}

void ControlChangeOverlay::paintEvent(QPaintEvent *event)
{
    QPainter painter(viewport());
    painter.setBrush(QBrush(QColor(128, 128, 255, 128)));
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing, true);
    for(const auto& item : lineItems)
    {

        QGraphicsLineItem* line = dynamic_cast<QGraphicsLineItem*>(item);
        QPointF l1 = mapFromScene( line->line().p1());
        QPointF l2 = mapFromScene( line->line().p2());
        QPointF points[4] = {QPointF(l1.x(),height()),QPointF(l2.x(),height()),
                             l2,l1};
        painter.drawPolygon(points,4);
    }
    QGraphicsView::paintEvent(event);
}

void ControlChangeOverlay::recalculateDT()
{
    listOfCC.clear();
    listOfCC.reserve(activeItems.size()*2);
    int counter = 0;
    int last = 0;
    for(const auto& var : activeItems)
    {
        auto item = static_cast<ControlChangeItem*>(var.second);
        if (item == leftItem || item == rightItem)
        {
            continue;
        }
        int event = (item->value << 16 |
                     ccType << 8  |
                     0xB0);
        if (counter == 0)
        {
            listOfCC.push_back(var.first);
            listOfCC.push_back(event);
            last = var.first;
            counter++;
        }
        else
        {
            listOfCC.push_back(var.first - last);
            listOfCC.push_back(event);
            last = var.first;
        }

    }
}

void ControlChangeOverlay::switchDrawModes()
{
    canDraw = !canDraw;
    if (canDraw)
    {
        for(const auto& item : activeItems)
        {
            item.second->hide();
        }
    }
    else
    {
        for(const auto& item : activeItems)
        {
            if (item.second == leftItem || item.second == rightItem)
            {
                continue;
            }
            item.second->show();
        }
    }
}
