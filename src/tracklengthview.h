#ifndef TRACKLENGTHVIEW_H
#define TRACKLENGTHVIEW_H

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
    void setScale(float x, bool needsReset, int wheelPos);
private:

protected:
    void paintEvent(QPaintEvent * event);
    void mouseDoubleClickEvent(QMouseEvent  *event);
    void mousePressEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);

};

#endif // TRACKLENGTHVIEW_H
