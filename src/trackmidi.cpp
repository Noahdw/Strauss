#include "trackmidi.h"

#include "src/trackview.h"
#include "src/pianoroll.h"
#include "src/plugintrackview.h"
#include "src/mainwindow.h"
#include "src/mastertrack.h"
#include "audiopluginfactory.h"
#include "audioplugin.h"
#include "vst2audioplugin.h"

TrackMidi::TrackMidi(MasterTrack *mTrack) : masterTrack(mTrack)
{
    _midiData.instrumentName = "New Track";
}

TrackMidi::~TrackMidi()
{
    _trackView->deleteLater();
    _pianoRoll->deleteLater();
}

void TrackMidi::setTrackView(TrackView *trackView)
{
    _trackView = trackView;
    //_trackView->pluginTrack->midiTrack = this;
}

void TrackMidi::setPianoRoll(PianoRoll *pianoRoll)
{
    _pianoRoll = pianoRoll;
    //_plugin.setPianoRollRef(pianoRoll);
}
//Will delete this class
void TrackMidi::prepareToDelete()
{
    masterTrack->removeTrack(this);
}

void TrackMidi::addPlugin(QString actualPath)
{

    if (actualPath == "") return;
    AudioPluginFactory factory(this);
    _plugin = factory.createAudioPlugin(actualPath);

}



