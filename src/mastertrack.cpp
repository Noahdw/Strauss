#include "mastertrack.h"
#include <algorithm>
#include "trackcontainer.h"
#include "pianorollcontainer.h"
#include "plugineditorcontainer.h"
#include "trackmidi.h"
#include "plugintrackview.h"


MasterTrack::MasterTrack()
{

}
TrackMidi* MasterTrack::addTrack()
{
    auto midiTrack = std::make_unique<TrackMidi>(this);
    auto trackView = trackContainer->addSingleView(midiTrack.get());
    midiTrack->setTrackView(trackView);
    auto pluginTrack = pluginEditorContainer->addTrack(trackView);
    trackView->pluginTrack = pluginTrack;
    auto pianoRoll = pianoRollContainer->addPianoRoll(trackView);
    midiTrack->setPianoRoll(pianoRoll);

    if (currentTrack() == nullptr)
        _currentTrack = midiTrack.get();

    _midiTracks.push_back(std::move(midiTrack));
    return midiTrack.get();
}

void MasterTrack::addMidiTrackFromProject(const::google::protobuf::RepeatedPtrField<MidiTrack> &pb_midi_track)
{

    for (int i = pb_midi_track.size() - 1; i >= 0 ; --i)
    {
        auto midiTrack = std::make_unique<TrackMidi>(this);
        auto masterPlug = pb_midi_track[i].master_plugin();
        auto trackView = trackContainer->addTrackFromLoadProject(pb_midi_track[i],midiTrack.get());
        midiTrack->setTrackView(trackView);
        auto pluginTrack = pluginEditorContainer->addTrack(trackView);
        trackView->pluginTrack = pluginTrack;
        auto pianoRoll = pianoRollContainer->addPianoRoll(trackView);
        midiTrack->setPianoRoll(pianoRoll);
        if (masterPlug.IsInitialized())
        {
            midiTrack->addPlugin(QString::fromStdString(masterPlug.plugin_url()));
            if(midiTrack->plugin() != nullptr)
                midiTrack->masterPlugin()->setPluginState(masterPlug.program_bank());
        }

        if (currentTrack() == NULL) {
            _currentTrack = midiTrack.get();
        }
        _midiTracks.push_back(std::move(midiTrack));
    }
}

/* Adds to a queue of tracks that the Audio Engine will delete
 * when it is safe to do so.
 * Any class can call this one.
 */
void MasterTrack::removeTrack(TrackMidi *track)
{
    _tracksToRemove.enqueue(track);
}
/* Must only be called by the Audio Engine when it is safe to remove.
 *
 */
void MasterTrack::unsafeRemoveTrack(TrackMidi* track)
{
    _midiTracks.erase(std::remove_if(_midiTracks.begin(),
                                     _midiTracks.end(),
                                     [&](std::unique_ptr<TrackMidi> const &t)
                                     {
                                         return t.get() == track;
                                     }));
}

QQueue<TrackMidi *> &MasterTrack::tracksToRemove()
{
    return _tracksToRemove;
}

void MasterTrack::setCurrentTrack(TrackMidi* trackMidi)
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
