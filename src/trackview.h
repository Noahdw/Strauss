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
    TrackView(mTrack *track,QWidget* parent = 0);
    void folderViewItemDoubleClicked(QString filepath, QString name);

    mTrack *track;
    pluginHolder plugin;
    TrackMidiView *trackMidiView;
    int id;
    int randomRed = 0;
    int randomGreen = 0;
    int randomBlue = 0;
        QString instrumentName;
        PluginTrackView *pluginTrack;
private:
    QLineEdit *instrumentLabel;
    QVBoxLayout *vlayout;
    QCheckBox *muteBox;
    QCheckBox *recordBox;
    QPushButton * showButton;
    bool canEditLine = false;
    const int widgetWidth = 150;


signals:
    void trackClickedOn(int id);

private slots:
    void notifyMuteChange(int state);
    void notifyRecordingChange(int state);
    void ShowContextMenu(const QPoint &pos);
    void showPlugin();
    void renameTrack();

protected:
    void paintEvent(QPaintEvent * event);
    void mousePressEvent(QMouseEvent *event);
    bool eventFilter(QObject *target, QEvent *event);
};

#endif // TRACKVIEW_H
