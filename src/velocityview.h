#ifndef VELOCITYVIEW_H
#define VELOCITYVIEW_H
class TrackView;

#include <QWidget>
#include <QPainter>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QMenu>


class VelocityView : public QGraphicsView{
    Q_OBJECT

public:
    VelocityView(QWidget *parent =0);
    void setScale(float x,bool needsReset,int wheelPos);
    void updateItems(int start, int velocity,int note, bool adding);
    void updateTrackOfItems(TrackView *track);
    void onPianoRollResized(float x);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QGraphicsScene *scene;


};

#endif // VELOCITYVIEW_H
