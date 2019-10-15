#include "trackmidi.h"

#include "src/trackwidget.h"
#include "src/pianorollwidget.h"
#include "src/plugintrackview.h"
#include "src/mainwindow.h"
#include "src/mastertrack.h"
#include "audiopluginfactory.h"
#include "audioplugin.h"
#include "vst2audioplugin.h"
#include "controlchange.h"

TrackMidi::TrackMidi(MasterTrack *mTrack) : masterTrack(mTrack)
{
    _midiData.instrumentName = "New Track";
    //_controlChange = new ControlChange(this);
}

TrackMidi::~TrackMidi()
{
    _trackView->deleteLater();
}

void TrackMidi::setTrackView(TrackWidget *trackView)
{
    _trackView = trackView;
    //_trackView->pluginTrack->midiTrack = this;
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
    if(_plugin != nullptr)
        _plugin->setCanProcess(true);

}
void TrackMidi::addMidiEventToPlayback(uchar status,uchar note, uchar velocity, qreal currentTick)
{
    if (masterPlugin())
        masterPlugin()->addMidiEvent(status,note,velocity,currentTick);
}

MidiData *TrackMidi::ccAt(int index)
{
    if(_cc.count(index) == 0)
    {
        qDebug() << "CC at " << index << " does not exist";
        return nullptr;
    }
    return &_cc[index];
}

bool TrackMidi::addCC(int index)
{
    if(_cc.count(index) != 0)
    {
        qDebug() << "CC at " << index << " already exists";
        return false;
    }
    _cc[index] = MidiData();
    return true;
}



