#ifndef MASTERTRACK_H
#define MASTERTRACK_H
#include <QList>
#include <QQueue>
#include "midiinter.pb.h"

class TrackContainer;
class PianoRollContainer;
class PluginEditorContainer;
class TrackMidi;
class TrackDirector;

typedef std::shared_ptr<TrackMidi> TrackMidiPtr;

class MasterTrack
{
public:
    MasterTrack();
    TrackMidi *addTrack();
    const std::vector<TrackMidiPtr> &midiTracks() {return _midiTracks;}
    void addMidiTrackFromProject(const::google::protobuf::RepeatedPtrField<MidiTrack> &pb_midi_track);
    void removeTrack(TrackMidi *track);
    void unsafeRemoveTrack(TrackMidi *track);
    void unsafeRemoveAllTracks();
    QQueue<TrackMidi *> &tracksToRemove();
    void setCurrentTrack(TrackMidi *trackMidi);
    TrackMidi *currentTrack(){ return mCurTrack;}

    void updateTrackPositions(bool isPaused, bool isRestart, int custom);
    void initializeDependencies(TrackDirector* trackDirector,PluginEditorContainer *pEditorContainer);
private:
    std::vector<TrackMidiPtr> _midiTracks;
    TrackMidi *mCurTrack = NULL;

    TrackDirector* director;
    PluginEditorContainer *pluginEditorContainer;
    QQueue<TrackMidi*> mTracksToRemove;
};

#endif // MASTERTRACK_H
