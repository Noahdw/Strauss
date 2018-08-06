#ifndef TRACKVIEW_H
#define TRACKVIEW_H

class TrackMidiView;
class PluginTrackView;

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
#include <QLineEdit>
#include <QPushButton>


class TrackView: public QFrame
{
    Q_OBJECT

public:
    TrackView(mTrack *track, TrackMidiView *trackMidiView, QWidget* parent = 0);
    void addPluginFromPath(QString filepath, QString name, QString actualPath);
    void addPluginFromLoadProject(QString filepath);
    TrackMidiView *getTrackMidiView();
    mTrack *track;
    pluginHolder plugin;
    int id;
    int randomRed = 0;
    int randomGreen = 0;
    int randomBlue = 0;
    QString instrumentName;
    PluginTrackView *pluginTrack;
protected:
    void paintEvent(QPaintEvent * event);
    void mousePressEvent(QMouseEvent *event);
    bool eventFilter(QObject *target, QEvent *event);
private:
    QLineEdit *instrumentLabel;
    QVBoxLayout *vlayout;
    QCheckBox *muteBox;
    QCheckBox *recordBox;
    QPushButton * showButton;
    TrackMidiView *track_midi_view;
    QBrush brush;
    bool canEditLine = false;
    const int widgetWidth = 100;

signals:
    void trackClickedOn(int id);

private slots:
    void notifyMuteChange(int state);
    void notifyRecordingChange(int state);
    void ShowContextMenu(const QPoint &pos);
    void showPlugin();
    void renameTrack();

};

#endif // TRACKVIEW_H
