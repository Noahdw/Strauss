#ifndef TRACKMIDI_H
#define TRACKMIDI_H

class TrackView;
#include "src/midimanager.h"

#include <QList>
class PianoRoll;

class MasterTrack;
class AudioPlugin;
class Vst2AudioPlugin;
class TrackMidi
{
public:
    TrackMidi(MasterTrack* mTrack);

    ~TrackMidi();
    void setTrackView(TrackView *trackView);
    void setPianoRoll(PianoRoll *pianoRoll);
    void prepareToDelete();
    void addPlugin(QString actualPath);

    void setCanRecord(bool state) {_canRecord = state;}
    bool canRecord() {return _canRecord;}
    void setMuted(bool state) {_muted = state;}
    bool muted() {return _muted;}

    TrackView*  trackView(){return _trackView;}
    MidiData*   midiData(){return &_midiData;}
    PianoRoll*  pianoRoll(){return _pianoRoll;}
    //redundent for now, not sure which one will make for sense in the future
    AudioPlugin* plugin(){return _plugin.get();}
    AudioPlugin* masterPlugin(){return _plugin.get();}
    QList<Vst2AudioPlugin*> effectPlugins; // TODO

private:
    TrackView * _trackView;
    MidiData  _midiData;
    PianoRoll * _pianoRoll;
    std::unique_ptr<AudioPlugin> _plugin;
    MasterTrack *masterTrack;
    bool _canRecord = false;
    bool _muted = false;

};

#endif // TRACKMIDI_H
