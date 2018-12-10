#ifndef MASTERTRACK_H
#define MASTERTRACK_H

class TrackContainer;
class PianoRollContainer;
class PluginEditorContainer;
class TrackMidi;
#include <QList>
#include "midiinter.pb.h"
class MasterTrack
{
public:
    MasterTrack();
    TrackMidi *addTrack();
    void addMidiTrackFromProject(const::google::protobuf::RepeatedPtrField<MidiTrack> &pb_midi_track);
    QList<TrackMidi*> midiTracks;
    void setCurrentTrack(TrackMidi *trackMidi);
    TrackMidi *currentTrack(){return _currentTrack;}
    void initializeDependencies(TrackContainer *tContainer, PianoRollContainer *pContainer, PluginEditorContainer *pEditorContainer);
private:
    TrackMidi *_currentTrack = NULL;
    TrackContainer *trackContainer;
    PianoRollContainer *pianoRollContainer;
    PluginEditorContainer *pluginEditorContainer;
};

#endif // MASTERTRACK_H
