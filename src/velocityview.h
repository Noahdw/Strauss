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
    void addOrRemoveVelocityViewItem(int start, int velocity,int note, bool adding);
    void changeVelocityViewItemPosition(int oldPos,int newPos, int oldNote,int newNote);
    void populateVelocityViewFromTrack(TrackView *track);
    void onPianoRollResized(float x);
    TrackView * trackView;
protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    QGraphicsScene *scene;


};

#endif // VELOCITYVIEW_H
