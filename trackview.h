#ifndef TRACKVIEW_H
#define TRACKVIEW_H

class TrackMidiView;

#include <QWidget>
#include <QPainter>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QMenu>
#include <QLabel>
#include <QBoxLayout>
#include <QGraphicsRectItem>
#include <QFrame>
#include <vst2hostcallback.h>



class TrackView: public QFrame
{
    Q_OBJECT

public:
    TrackView(mTrack *track,QWidget* parent = 0);
    int id;
    mTrack *track;
    pluginHolder plugin;
    TrackMidiView *trackMidiView;
private:
    QString instrumentName;
    QLabel *instrumentLabel;
    QVBoxLayout *vlayout;
    const int widgetWidth = 70;
    int randomRed = 0;
    int randomGreen = 0;
    int randomBlue = 0;
signals:
    void trackClickedOn(int id);
protected:
    void paintEvent(QPaintEvent * event);
    void mousePressEvent(QMouseEvent *event);
};

#endif // TRACKVIEW_H
