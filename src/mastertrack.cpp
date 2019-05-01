#include "mastertrack.h"
#include <algorithm>
#include "trackcontainer.h"
#include "pianorollcontainer.h"
#include "plugineditorcontainer.h"
#include "trackmidi.h"
#include "plugintrackview.h"
#include "pianoroll.h"
#include "trackdirector.h"
MasterTrack::MasterTrack()
{

}
TrackMidi* MasterTrack::addTrack()
{
    auto midiTrack = std::make_unique<TrackMidi>(this);
    auto trackView = director->getTrackContainer()->addSingleView(midiTrack.get());
    midiTrack->setTrackView(trackView);
    auto pluginTrack = pluginEditorContainer->addTrack(trackView);
    trackView->pluginTrack = pluginTrack;

    if (_currentTrack == nullptr)
        _currentTrack = midiTrack.get();

    _midiTracks.push_back(std::move(midiTrack));
    pluginEditorContainer->addTrack(trackView);
    return currentTrack();
}

void MasterTrack::addMidiTrackFromProject(const::google::protobuf::RepeatedPtrField<MidiTrack> &pb_midi_track)
{

    for (int i = pb_midi_track.size() - 1; i >= 0 ; --i)
    {
        auto midiTrack = std::make_unique<TrackMidi>(this);
        auto masterPlug = pb_midi_track[i].master_plugin();
        auto trackView = director->getTrackContainer()->addTrackFromLoadProject(pb_midi_track[i],midiTrack.get());
        midiTrack->setTrackView(trackView);
        auto pluginTrack = pluginEditorContainer->addTrack(trackView);
        trackView->pluginTrack = pluginTrack;
        if (masterPlug.IsInitialized())
        {
            midiTrack->addPlugin(QString::fromStdString(masterPlug.plugin_url()));
            if(midiTrack->plugin() != nullptr)
                midiTrack->masterPlugin()->setPluginState(masterPlug.program_bank());
        }

        if (_currentTrack == nullptr) {
            _currentTrack = midiTrack.get();
        }
        _midiTracks.push_back(std::move(midiTrack));
        pluginEditorContainer->addTrack(trackView);
    }
    director->getPianoRollContainer()->restoreTrack(currentTrack());
}

/* Adds to a queue of tracks that the Audio Engine will delete
 * when it is safe to do so.
 * Any class can call this one.
 */
void MasterTrack::removeTrack(TrackMidi *track)
{
    _tracksToRemove.enqueue(track);
    if(currentTrack() == track)
    {
        for(const auto& t : midiTracks())
        {
            if(t.get() != track)
            {
                director->setActiveTrack(t.get());
                return;
            }

        }
    }
}
/* Must only be called by the Audio Engine when it is safe to remove.
 *
 */
void MasterTrack::unsafeRemoveTrack(TrackMidi* track)
{
    //director->getTrackContainer().
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
    //_midiEditor->setActiveTrack(trackMidi);
}

void MasterTrack::updateTrackPositions(bool isPaused, bool isRestart, int custom)
{
    director->getPianoRollContainer()->pianoRoll()->updateSongTrackerPos(isPaused,isRestart,custom);
    for(const auto& t : midiTracks())
    {
        // t.get()->pianoRoll()->updateSongTrackerPos(isPaused,isRestart,custom);
    }

}
/*
    TODO: Need a better way to do this, don't like the cyclic dependency
*/
void MasterTrack::initializeDependencies(TrackDirector* trackDirector, PluginEditorContainer *pEditorContainer)
{
    director = trackDirector;
    pluginEditorContainer = pEditorContainer;

}
