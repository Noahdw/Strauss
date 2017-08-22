#ifndef TRACKVIEW_H
#define TRACKVIEW_H
#include <QWidget>
#include <QPainter>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QMenu>
#include <midimanager.h>
class TrackView: public QGraphicsView{
    Q_OBJECT

public:
    TrackView(QWidget* parent = 0);

public slots:
    void trackViewChanged(mSong *song);

private:
QGraphicsScene *scene;
};

#endif // TRACKVIEW_H
