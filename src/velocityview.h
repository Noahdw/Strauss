#ifndef VELOCITYVIEW_H
#define VELOCITYVIEW_H
class TrackView;

#include <QWidget>
#include <QPainter>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QMenu>
#include <src/midimanager.h>


class VelocityView : public QGraphicsView{
    Q_OBJECT

public:
    VelocityView(TrackView *trackView,QWidget *parent =0);
    void setScale(float x,bool needsReset,int wheelPos);
    void addOrRemoveVelocityViewItem(int start, int velocity,int note, bool adding);
    void changeVelocityViewItemPosition(int oldPos,int newPos, int oldNote,int newNote);
    void populateVelocityViewFromTrack(TrackView *track);
    void onPianoRollResized(float x);
    mTrack *getTrack();
protected:
    void paintEvent(QPaintEvent *event);

private:
    QGraphicsScene *scene;
    TrackView *track_view;

};

#endif // VELOCITYVIEW_H
