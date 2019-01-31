#ifndef TRACKVIEW_H
#define TRACKVIEW_H

class TrackMidiView;
class PluginTrackView;
class TrackContainer;
class TrackMidi;

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
#include <QSlider>
#include <QComboBox>

class TrackView: public QFrame
{
    Q_OBJECT

public:
    TrackView(TrackMidi *midiTrack, TrackMidiView *_trackMidiView, TrackContainer *_trackContainer, QWidget* parent = 0);
    ~TrackView();
    void deleteTrack();
    void deselect();
    QString getTrackName() const;
    TrackMidiView *getTrackMidiView();
    TrackMidi* midiTrack(){return _midiTrack;}
    int id;
    int randomRed = 0;
    int randomGreen = 0;
    int randomBlue = 0;
    QString instrumentName;
    PluginTrackView *pluginTrack;
    QLineEdit *instrumentLabel;
    QComboBox *comboBox;
protected:
    void paintEvent(QPaintEvent * event);
    void mousePressEvent(QMouseEvent *event);
    bool eventFilter(QObject *target, QEvent *event);
private:
    TrackMidi* _midiTrack;

    QVBoxLayout *vlayout;
    QCheckBox *muteBox;
    QCheckBox *recordBox;
    QPushButton * showButton;
    TrackMidiView *_trackMidiView;
    TrackContainer *_trackContainer;

    QBrush brush;
    bool canEditLine = false;
    const int widgetWidth = 100;

signals:
    void trackClickedOn(TrackView *track_View);

private slots:
    void notifyMuteChange(int state);
    void notifyRecordingChange(int state);
    void ShowContextMenu(const QPoint &pos);
    void showPlugin();
    void renameTrack();
    void comboBoxIdChanged(int index);

};

#endif // TRACKVIEW_H
