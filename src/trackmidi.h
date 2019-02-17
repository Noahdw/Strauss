#ifndef TRACKMIDI_H
#define TRACKMIDI_H

class TrackView;
#include "src/midimanager.h"
#include <QList>
class PianoRoll;
class Vst2HostCallback;
class MasterTrack;

class TrackMidi
{
public:
    TrackMidi(MasterTrack* mTrack);
    ~TrackMidi();
    void setTrackView(TrackView *trackView);
    void setPianoRoll(PianoRoll *pianoRoll);
    void setMasterPlugin(Vst2HostCallback *vst2HostCallBack) {_plugin = vst2HostCallBack;}
    Vst2HostCallback *loadPlugin(QString actualPath);
    TrackView*  trackView(){return _trackView;}
    MidiData*   midiData(){return &_midiData;}
    PianoRoll*  pianoRoll(){return _pianoRoll;}
    //redundent for now, not sure which one will make for sense in the future
    Vst2HostCallback* plugin(){return _plugin;}
    Vst2HostCallback* masterPlugin(){return _plugin;}
    QList<Vst2HostCallback*> effectPlugins;

private:
    TrackView * _trackView;
    MidiData  _midiData;
    PianoRoll * _pianoRoll;
    Vst2HostCallback *_plugin;
    MasterTrack *masterTrack;
};

#endif // TRACKMIDI_H
