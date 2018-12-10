#ifndef TRACKMIDI_H
#define TRACKMIDI_H

class TrackView;
#include "src/midimanager.h"
#include <QList>
class PianoRoll;
class Vst2HostCallback;


class TrackMidi
{
public:
    TrackMidi();
    void setTrackView(TrackView *trackView);
    void setPianoRoll(PianoRoll *pianoRoll);
    void setMasterPlugin(Vst2HostCallback *vst2HostCallBack) {plugin = vst2HostCallBack;}
    void deleteTrack();
    Vst2HostCallback *loadPlugin(QString actualPath);
    TrackView *trackView(){return _trackView;}
    MidiData  *midiData(){return &_midiData;}
    PianoRoll *pianoRoll(){return _pianoRoll;}
    Vst2HostCallback *masterPlugin(){return plugin;}
    QList<Vst2HostCallback*> effectPlugins;

private:
    TrackView * _trackView;
    MidiData  _midiData;
    PianoRoll * _pianoRoll;
    Vst2HostCallback *plugin;
};

#endif // TRACKMIDI_H
