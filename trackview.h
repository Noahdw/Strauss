#ifndef TRACKVIEW_H
#define TRACKVIEW_H
#include <QWidget>
#include <QPainter>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QMenu>
#include <midimanager.h>
#include <QLabel>
#include <QBoxLayout>
#include <QGraphicsRectItem>
#include <QFrame>
class TrackView: public QFrame
{
    Q_OBJECT

public:
    TrackView(mTrack track,QWidget* parent = 0);
    int id;
    mTrack track;

private:
    QString instrumentName;
    QLabel *instrumentLabel;
    QVBoxLayout *vlayout;
    const int widgetWidth = 70;
signals:
    void trackClickedOn(int id);
protected:
    void paintEvent(QPaintEvent * event);
    void mousePressEvent(QMouseEvent *event);
};

#endif // TRACKVIEW_H
