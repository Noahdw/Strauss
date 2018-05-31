#ifndef TRACKLENGTHVIEW_H
#define TRACKLENGTHVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

class TrackLengthView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit TrackLengthView(QWidget *parent = nullptr);
    QGraphicsScene * scene;
signals:

public slots:

protected:
    void paintEvent(QPaintEvent * event);
    void mouseDoubleClickEvent(QMouseEvent  *event);
    void mousePressEvent(QMouseEvent *event);
};

#endif // TRACKLENGTHVIEW_H
