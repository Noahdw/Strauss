#ifndef TRACKCONTAINER_H
#define TRACKCONTAINER_H

class TrackMidiView;
class PluginEditorContainer;

#include <QObject>
#include <QWidget>
#include <src/midimanager.h>
#include <src/trackview.h>
#include <QVBoxLayout>
#include <QScrollArea>
#include <src/pianorollcontainer.h>
#include <QSpacerItem>
#include <QSplitter>
#include <QKeyEvent>
#include <QPainter>

class TrackContainer : public QWidget
{
    Q_OBJECT
public:
    TrackContainer();
    void setPianoRollReference(PianoRollContainer *prc);
    void addSingleView(TrackView *view);
    PluginEditorContainer *pluginEditorContainer;
signals:
void switchControlChange();
public slots:
    void addTrackView(mSong *song);

signals:
    void addPianoRoll(TrackView *track);
    void requestTrackChange(int id);

private:
    QSplitter *vSplitter;
    QVBoxLayout *vLayout;
    QHBoxLayout *hLayout;
    QScrollArea *scrollArea;

protected:
   void mousePressEvent(QMouseEvent *event);
   void keyPressEvent(QKeyEvent * event);
   void paintEvent(QPaintEvent *event);
   PianoRollContainer *prcontainer;
};

#endif // TRACKCONTAINER_H
