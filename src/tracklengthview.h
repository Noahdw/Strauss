#ifndef TRACKLENGTHVIEW_H
#define TRACKLENGTHVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QScrollBar>

class PianoRollWidget;
class PianoRollContainer;
class TrackMidi;

class TrackLengthView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit TrackLengthView(PianoRollContainer* p);
    PianoRollContainer* container(){return _container;}
    QGraphicsScene * scene;
    void setScale(QMatrix matrix, qreal scaleFactor);
    void initTrackLengthView(QRectF sceneRect, float scaleX);
    void restoreTrack(TrackMidi* midiTrack);
    double scaleFactor = 1;
protected:
    void paintEvent(QPaintEvent * event);
    void mouseDoubleClickEvent(QMouseEvent  *event);
    void mousePressEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent *event);
private:
    double minSpacing = 20.0;

    PianoRollContainer* _container;
};

#endif // TRACKLENGTHVIEW_H
