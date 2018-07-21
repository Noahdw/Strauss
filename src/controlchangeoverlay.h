#ifndef CONTROLCHANGEOVERLAY_H
#define CONTROLCHANGEOVERLAY_H

class ControlChangeItem;
class CollisionItem;

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWidget>
#include <QPainter>
#include <QApplication>
#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsItem>
#include <QList>
#include <QKeyEvent>
#include <QRubberBand>
#include <QEvent>
#include <QGraphicsSceneEvent>
#include <QPointF>
#include <map>
class ControlChangeOverlay  : public QGraphicsView{
    Q_OBJECT

public:
    ControlChangeOverlay(QWidget *parent = 0);
    void createLineConnector();
    void removeSelectedItems();
    void removeCollidingItems(QList<QGraphicsItem*> &items);
    void addPoint(int x, int value);
    void fitIntoView();
    void recalculateDT();

    bool eventFilter(QObject *target, QEvent *event);
    QGraphicsScene * scene = nullptr;
    std::map<int,QGraphicsItem*> activeItems;
    bool canDraw = false;
    std::vector<int> listOfCC;
    int ccType;
protected:
    void showEvent(QShowEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent *event);
private:
    ControlChangeItem *leftItem;
    ControlChangeItem *rightItem;
    CollisionItem *collisionItem;
    bool firstShow = true;
    QPoint origin;
    std::vector<QGraphicsLineItem*> lineItems;
    QRubberBand *rubberBand;
    QGraphicsRectItem * rectItem;
};

#endif // CONTROLCHANGEOVERLAY_H
