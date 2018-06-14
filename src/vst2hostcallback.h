#ifndef VST2HOSTCALLBACK_H
#define VST2HOSTCALLBACK_H

class PianoRoll;

#include "SDK/aeffectx.h"
#include <SDK/aeffect.h>
#include <SDK/vstfxstore.h>
#include <src/midimanager.h>
#include <qvector.h>
#include <QObject>

struct EventToAdd
{
    int note = 0;
    bool eventOn = false;
    bool hasEventToAdd = false;
};

class Vst2HostCallback : public QObject
{
Q_OBJECT
public:
    Vst2HostCallback(mTrack *track);
    AEffect* loadPlugin(char* fileName);
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

    void setPianoRollRef(PianoRoll *piano);
    
    EventToAdd eventToAdd;
    unsigned int blocksize = 256;
    float sampleRate = 44100.0f;
    bool canPlay = false;
    bool isMuted = false;
    bool isPaused = false;
    void turnOffAllNotes(AEffect *plugin);

private:
    QVector<int> *noteList;
    VstEvents *events;
    mTrack *track;
    LPCSTR APPLICATION_CLASS_NAME = (LPCSTR)"MIDIHOST";

    HMODULE hinst;
    float **outputs;
    float ** inputs;
    uint numChannels = 2;

    int noteVecPos = 0;
    const uint maxNotes = 256;
    int TPQN = MidiManager::TPQN;
    int BPM = 500000;
    float samplesPerTick = 0;
    uint framesTillBlock = 0;
    bool hasReachedEnd = false;
    PianoRoll *pianoroll;
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
