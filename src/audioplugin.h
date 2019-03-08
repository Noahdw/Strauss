#ifndef AUDIOPLUGIN_H
#define AUDIOPLUGIN_H

#include <QObject>
#include <deque>
#include <queue>
class TrackMidi;

struct EventToAdd
{
    uchar status = 0x90;
    uchar note = 0;
    bool eventOn = false;
    bool hasEventToAdd = false;
    qreal timeInTicks = 0;
    uchar velocity;
};
/*
    Base class for audio plugins
*/
class AudioPlugin
{
public:
    AudioPlugin(TrackMidi* track);
    virtual void processAudio(float** input, float** output, int numFrames) =0;
    virtual void processMidi() =0;
    virtual bool loadPlugin(QString path, QString pluginName) =0;
    virtual void unloadPlugin() =0;
    virtual void showPlugin() =0;
    virtual void hidePlugin() {}
    virtual void setPluginState(const std::string &chunk) {}
    virtual void addMidiEvent(uchar status, uchar note, uchar velocity, qreal currentTick) {}
    virtual void restartPlayback(){}
    virtual void setBlockSize(int size) {}
    virtual void turnOffAllNotes() {}
    virtual void setCustomPlackbackPos(int pos) {}
    virtual void exportAudioInit() {}
    virtual int exportAudioBegin(float** output,int blocksize) =0;
    virtual void exportAudioEnd() {}
    virtual std::string savePluginState() const =0;
    bool canProcess() {return _canProcess;}
    TrackMidi* midiTrack() {return _midiTrack;}
    virtual void setCanProcess(bool state) {_canProcess = state;}
    std::queue<EventToAdd> midiEventQueue;
    std::deque<EventToAdd> recordedMidiEventDeque;
    QString pluginPath;
       TrackMidi* _midiTrack;
private:

   bool _canProcess = false;
};

#endif // AUDIOPLUGIN_H
