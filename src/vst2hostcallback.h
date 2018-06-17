#ifndef VST2HOSTCALLBACK_H
#define VST2HOSTCALLBACK_H

class PianoRoll;

#include "SDK/aeffectx.h"
#include <SDK/aeffect.h>
#include <SDK/vstfxstore.h>
#include <src/midimanager.h>
#include <qvector.h>
#include <QObject>
#include <queue>
struct EventToAdd
{
    int note = 0;
    bool eventOn = false;
    bool hasEventToAdd = false;
    uchar velocity;
};

class Vst2HostCallback : public QObject
{
    Q_OBJECT
public:
    Vst2HostCallback(mTrack *track);
    AEffect* loadPlugin(char* fileName);
    bool canRecord();
    int configurePluginCallbacks(AEffect *plugin);
    void startPlugin(AEffect *plugin);
    void initializeIO();
    void processAudio(AEffect *plugin, float **inputs, float **outputs,
                      long numFrames);
    void silenceChannel(float **channelData, int numChannels, long numFrames);
    void processMidi(AEffect *plugin);
    void initializeMidiEvents();
    void restartPlayback();
    void pauseOrResumePlayback(bool isResume);
    void addMidiEvent(uchar note, uchar velocity);
    void setPianoRollRef(PianoRoll *piano);
    void setCanRecord(bool canRec);
    void turnOffAllNotes(AEffect *plugin);

    EventToAdd eventToAdd;
    std::queue<EventToAdd> midiEventQueue;
    uint blocksize = 256;
    float sampleRate = 44100.0f;
    bool canPlay = false;
    bool isMuted = false;
    bool isPaused = false;

private:
    PianoRoll *pianoroll;
    VstEvents *events;
    mTrack *track;
    QVector<int> *noteList;
    LPCSTR APPLICATION_CLASS_NAME = (LPCSTR)"MIDIHOST";
    HMODULE hinst;
    bool canRecording = false;
    bool hasReachedEnd = false;
    int TPQN = MidiManager::TPQN;
    int BPM = 500000;
    int noteVecPos = 0;
    const uint maxNotes = 256;
    uint numChannels = 2;
    uint framesTillBlock = 0;
    float **outputs;
    float ** inputs;
    float samplesPerTick = 0;

public slots:
    void setCustomPlackbackPos(int playbackPos);

};

struct pluginHolder
{
    Vst2HostCallback *host =NULL;
    AEffect *effect =NULL;
};

//from http://teragonaudio.com/article/How-to-make-your-own-VST-host.html
// C callbacks

// Main host callback

//extern "C"{
//VstIntPtr VSTCALLBACK hostCallback(AEffect *effect, VstInt32 opcode,
// VstInt32 index, VstInt32 value, void *ptr, float opt);
//}


// Plugin's entry point
typedef AEffect* (*vstPluginFuncPtr)(audioMasterCallback host);
// Plugin's dispatcher function
typedef VstIntPtr (*dispatcherFuncPtr)(AEffect *effect, VstInt32 opCode,
                                       VstInt32 index, VstInt32 value, void *ptr, float opt);
// Plugin's getParameter() method
typedef float (*getParameterFuncPtr)(AEffect *effect, VstInt32 index);
// Plugin's setParameter() method
typedef void (*setParameterFuncPtr)(AEffect *effect, VstInt32 index, float value);
// Plugin's processEvents() method
typedef VstInt32 (*processEventsFuncPtr)(VstEvents *events);
// Plugin's process() method
typedef void (*processFuncPtr)(AEffect *effect, float **inputs,
                               float **outputs, VstInt32 sampleFrames);
#endif // VST2HOSTCALLBACK_H
