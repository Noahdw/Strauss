#ifndef TRACKCONTAINER_H
#define TRACKCONTAINER_H

class TrackMidiView;
class PluginEditorContainer;
class TrackMidi;
class MasterTrack;

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

class TrackContainer : public QFrame
{
    Q_OBJECT
public:
    TrackContainer(PluginEditorContainer *pluginEditorContainer, PianoRollContainer * pianoRollContainer, MasterTrack * masterTrack);
    TrackView *addTrackFromLoadProject(const MidiTrack &midi_track, TrackMidi *midiTrack);
    TrackView *addSingleView(TrackMidi *midiTrack);
    void deleteTrack(TrackView *trackView, TrackMidiView *midiView);
    int getNumTracks() const;
    void deleteAllTracks();
    void trackClicked(TrackView * trackView);
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

private:
    MasterTrack *masterTrack;
    PluginEditorContainer *pluginEditorContainer;
    PianoRollContainer    *pianoRollContainer;
    QSplitter   *vSplitter;
    QVBoxLayout *vLayout;
    QHBoxLayout *hLayout;
    QScrollArea *scrollArea;
};

#endif // TRACKCONTAINER_H
