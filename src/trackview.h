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
#include <src/vst2hostcallback.h>
#include <QCheckBox>



class TrackView: public QFrame
{
    Q_OBJECT

public:
    TrackView(mTrack *track,QWidget* parent = 0);
    int id;
    mTrack *track;
    pluginHolder plugin;
    TrackMidiView *trackMidiView;
    void folderViewItemDoubleClicked(QString filepath, QString name);
private:
    QString instrumentName;
    QLabel *instrumentLabel;
    QVBoxLayout *vlayout;
    QCheckBox *muteBox;
    const int widgetWidth = 70;
    int randomRed = 0;
    int randomGreen = 0;
    int randomBlue = 0;
signals:
    void trackClickedOn(int id);
private slots:
    void notifyMuteChange(int state);
protected:
    void paintEvent(QPaintEvent * event);
    void mousePressEvent(QMouseEvent *event);
};

#endif // TRACKVIEW_H
