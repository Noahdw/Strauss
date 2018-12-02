#ifndef TRACKLENGTHVIEW_H
#define TRACKLENGTHVIEW_H
class PianoRoll;
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QScrollBar>


class TrackLengthView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit TrackLengthView(QWidget *parent = nullptr);
    QGraphicsScene * scene;
    void setScale(float x, bool needsReset, int wheelPos, double _scaleFactor);
    void initTrackLengthView(QRectF sceneRect, float scaleX);
    PianoRoll *pianoRoll;
private:
    double minSpacing = 20.0;
    double scaleFactor = 1;
protected:
    void paintEvent(QPaintEvent * event);
    void mouseDoubleClickEvent(QMouseEvent  *event);
    void mousePressEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent *event);
};

#endif // TRACKLENGTHVIEW_H
