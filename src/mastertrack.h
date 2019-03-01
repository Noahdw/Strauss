#ifndef MASTERTRACK_H
#define MASTERTRACK_H
#include <QList>
#include <QQueue>
#include "midiinter.pb.h"
class TrackContainer;
class PianoRollContainer;
class PluginEditorContainer;
class TrackMidi;

class MasterTrack
{
public:
    MasterTrack();
    TrackMidi *addTrack();
    const std::vector<std::unique_ptr<TrackMidi>> &midiTracks() {return _midiTracks;}
    void addMidiTrackFromProject(const::google::protobuf::RepeatedPtrField<MidiTrack> &pb_midi_track);
    void removeTrack(TrackMidi *track);
    void unsafeRemoveTrack(TrackMidi *track);
    QQueue<TrackMidi *> &tracksToRemove();
    void setCurrentTrack(TrackMidi *trackMidi);
    TrackMidi *currentTrack(){return _currentTrack;}
    void initializeDependencies(TrackContainer *tContainer, PianoRollContainer *pContainer, PluginEditorContainer *pEditorContainer);
private:
    std::vector<std::unique_ptr<TrackMidi>> _midiTracks;
    TrackMidi *_currentTrack = NULL;
    TrackContainer *trackContainer;
    PianoRollContainer *pianoRollContainer;
    PluginEditorContainer *pluginEditorContainer;
    QQueue<TrackMidi*> _tracksToRemove;
};

#endif // MASTERTRACK_H
