#ifndef TRACKCONTAINER_H
#define TRACKCONTAINER_H

class TrackMidiView;
class PluginEditorContainer;
class TrackMidi;
class MasterTrack;

#include <QObject>
#include <QWidget>
#include <QScrollArea>
#include <src/midimanager.h>
#include <src/trackwidget.h>
#include <QVBoxLayout>
#include <QScrollArea>
#include <src/pianorollcontainer.h>
#include <QSpacerItem>
#include <QSplitter>
#include <QKeyEvent>
#include <QPainter>
#include "src/plugineditorcontainer.h"
#include "src/midiinter.pb.h"

class TrackDirector;

class TrackContainer : public QScrollArea
{
    Q_OBJECT
public:
    TrackContainer(MasterTrack *masterTrack, TrackDirector* tDirector);
    TrackWidget *addTrackFromLoadProject(const MidiTrack &midi_track, TrackMidi *midiTrack);
    TrackWidget *addSingleView(TrackMidi *midiTrack);
    void deleteTrack(TrackWidget *trackView, TrackMidiView *midiView);
    int getNumTracks() const;
    void deleteAllTracks();
    void trackClicked(TrackWidget * trackView);
    std::vector<TrackWidget *> getTrackViews() const;
signals:
    void switchControlChange();
public slots:
    void addTrackView(const mSong &song);
signals:
    void addPianoRoll(TrackWidget *track);
    void requestTrackChange(int id);
protected:
    void keyPressEvent(QKeyEvent * event);

private:
    MasterTrack *_masterTrack;
    TrackDirector * director;
    //PluginEditorContainer *pluginEditorContainer;
    //PianoRollContainer    *pianoRollContainer;
    QSplitter   *vSplitter;
    QVBoxLayout *vLayout;
    QHBoxLayout *hLayout;
    QScrollArea *scrollArea;
};

#endif // TRACKCONTAINER_H
