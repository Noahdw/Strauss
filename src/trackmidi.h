#ifndef TRACKMIDI_H
#define TRACKMIDI_H

class TrackView;
#include "src/midimanager.h"

#include <QList>
class PianoRoll;

class MasterTrack;
class AudioPlugin;
class Vst2AudioPlugin;
class ControlChange;
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
    bool isMuted() {return _muted;}

    TrackView*  trackView(){return _trackView;}
    MidiData*   midiData(){return &_midiData;}
    PianoRoll*  pianoRoll(){return _pianoRoll;}\
    MidiData*   ccAt(int index);
    bool addCC(int index);
    std::map<int,MidiData> &cc() {return _cc;}
    //redundent for now, not sure which one will make for sense in the future
    AudioPlugin* plugin(){return _plugin.get();}
    AudioPlugin* masterPlugin(){return _plugin.get();}
    QList<Vst2AudioPlugin*> effectPlugins; // TODO
    ControlChange* _controlChange;
private:

    TrackView * _trackView;
    MidiData  _midiData;
    std::map<int,MidiData> _cc;
    PianoRoll* _pianoRoll;
    std::unique_ptr<AudioPlugin> _plugin;
    MasterTrack *masterTrack;
    bool _canRecord = false;
    bool _muted = false;

};

#endif // TRACKMIDI_H
