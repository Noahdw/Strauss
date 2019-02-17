#include "mastertrack.h"
#include "trackcontainer.h"
#include "pianorollcontainer.h"
#include "plugineditorcontainer.h"
#include "trackmidi.h"
#include "plugintrackview.h"
MasterTrack::MasterTrack()
{

}
TrackMidi *MasterTrack::addTrack()
{
    TrackMidi *midiTrack = new TrackMidi(this);
    auto trackView = trackContainer->addSingleView(midiTrack);
    midiTrack->setTrackView(trackView);
    auto pluginTrack = pluginEditorContainer->addTrack(trackView);
    trackView->pluginTrack = pluginTrack;
    auto pianoRoll = pianoRollContainer->addPianoRoll(trackView);
    midiTrack->setPianoRoll(pianoRoll);
    midiTracks.append(midiTrack);

    if (currentTrack() == NULL) {
        _currentTrack = midiTrack;
    }
    return midiTrack;
}

void MasterTrack::addMidiTrackFromProject(const::google::protobuf::RepeatedPtrField<MidiTrack> &pb_midi_track)
{

    for (int i = pb_midi_track.size() - 1; i >= 0 ; --i)
    {
        auto *midiTrack = new TrackMidi(this);
        auto masterPlug = pb_midi_track[i].master_plugin();
        auto trackView = trackContainer->addTrackFromLoadProject(pb_midi_track[i],midiTrack);
        midiTrack->setTrackView(trackView);
        auto pluginTrack = pluginEditorContainer->addTrack(trackView);
        trackView->pluginTrack = pluginTrack;
        auto pianoRoll = pianoRollContainer->addPianoRoll(trackView);
        midiTrack->setPianoRoll(pianoRoll);
        if (masterPlug.IsInitialized())
        {
            if (midiTrack->loadPlugin(QString::fromStdString(masterPlug.plugin_url())) != NULL) {
                midiTrack->masterPlugin()->setPluginState(midiTrack->masterPlugin()->effect,masterPlug.program_bank());
            }
        }

        if (currentTrack() == NULL) {
            _currentTrack = midiTrack;
        }
        midiTracks.append(midiTrack);
    }
}

void MasterTrack::removeTrack(TrackMidi *track)
{
    _tracksToRemove.enqueue(track);
}

QQueue<TrackMidi *> &MasterTrack::tracksToRemove()
{
    return _tracksToRemove;
}

void MasterTrack::setCurrentTrack(TrackMidi *trackMidi)
{
    Q_ASSERT(trackMidi != NULL);
    _currentTrack = trackMidi;
}
/*
    TODO: Need a better way to do this, too many classes are dependent on one another to use constructors
*/
void MasterTrack::initializeDependencies(TrackContainer *tContainer, PianoRollContainer *pContainer, PluginEditorContainer *pEditorContainer)
{
    trackContainer = tContainer;
    pianoRollContainer = pContainer;
    pluginEditorContainer = pEditorContainer;
}