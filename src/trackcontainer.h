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
#include "src/plugineditorcontainer.h"
#include "src/midiinter.pb.h"
class TrackContainer : public QWidget
{
    Q_OBJECT
public:
    TrackContainer(PluginEditorContainer *pluginEditorContainer, PianoRollContainer * pianoRollContainer);
    TrackView *addTrackFromLoadProject(const MidiTrack &midi_track, int totalDT);
    void addSingleView();
    void deleteTrack(TrackView *trackView, TrackMidiView *midiView);
    int getNumTracks() const;
    void deleteAllTracks();
    std::vector<TrackView *> getTrackViews() const;
signals:
    void switchControlChange();
public slots:
    void addTrackView(const mSong &song);
signals:
    void addPianoRoll(TrackView *track);
    void requestTrackChange(int id);
protected:
   void keyPressEvent(QKeyEvent * event);
   void paintEvent(QPaintEvent *event);
private:
    PluginEditorContainer *plugin_editor_container;
    PianoRollContainer    *piano_roll_container;
    QSplitter   *vSplitter;
    QVBoxLayout *vLayout;
    QHBoxLayout *hLayout;
    QScrollArea *scrollArea;
};

#endif // TRACKCONTAINER_H
