#ifndef VST2HOSTCALLBACK_H
#define VST2HOSTCALLBACK_H

class PianoRoll;
class PluginTrackView;
class TrackMidi;

#include <string>
#include <iostream>
#include <fstream>
#include "audioplugin.h"
#include "SDK/aeffectx.h"
#include <SDK/aeffect.h>
#include <SDK/vstfxstore.h>
#include <src/midimanager.h>
#include <src/midiinter.pb.h>
#include <qvector.h>
#include <QObject>
#include <queue>

typedef VstIntPtr (*dispatcherFuncPtr)(AEffect *effect, VstInt32 opCode,
                                       VstInt32 index, VstInt32 value, void *ptr, float opt);
struct Rect {
    short top;
    short left;
    short bottom;
    short right;
};



class Vst2AudioPlugin : public AudioPlugin
{

public:
     Vst2AudioPlugin(TrackMidi *track);

    ~Vst2AudioPlugin();
    bool loadPlugin(QString path, QString name);
    AEffect* LoadBridgedPlugin(char * szPath);
    int numParams(AEffect *effect) const;
    QString getParamName(AEffect *effect,int index);
    int configurePluginCallbacks();
    void startPlugin();
    void initializeIO();
    void processAudio(float **inputs, float **outputs,int numFrames);
    void silenceChannel(float **channelData, int numChannels, long numFrames);
    void processMidi();
    void initializeMidiEvents();
    void restartPlayback();
    void addMidiEvent(uchar status, uchar note, uchar velocity, qreal currentTick);
    void setPianoRollRef(PianoRoll *piano);
    void turnOffAllNotes();
    void showPlugin();
    void hidePlugin();
    void exportAudioInit();
    void setBlockSize(int blockSize);
    int exportAudioBegin(float **outputs,
                         int numFrames);
    void unloadPlugin();
    void exportAudioEnd();
    std::string savePluginState() const;
    void setPluginState(const std::string &chunk);
    EventToAdd eventToAdd;
    MidiData *track;
    int ccFramesTillBlock[128];
    int ccVecPos[128];
    PianoRoll *pianoroll;
    double trackVolume = 1.0;
    float pan = 0.0;
    float sampleRate = 44100.0f;
    bool canPlay = false;
    bool isMuted = false;
    bool isPaused = true;
    bool isMasterPlugin = false;
    bool shouldDelete = false;
    int noteVecPos = 0;
    QString pluginName;
    QString unique_plugin_id;
    PluginTrackView * masterPluginTrackView;
    TrackMidi *midiTrack;
    dispatcherFuncPtr dispatcher;
    AEffect *effect = NULL;
    void setCustomPlackbackPos(int playbackPos);
private:
    VstEvents *events;

    QVector<int> *noteList;

    HMODULE hinst;
    bool canRecording = false;
    bool hasReachedEnd = false;
    int TPQN = MidiManager::TPQN;
    const uint maxNotes = 256;
    VstMidiEvent *eventsHolder[256];
    uint numChannels = 2;
    int framesTillBlock = -1;
    float **outputs;
    float **inputs;
    float samplesPerTick = 0;

    HWND editor = nullptr;
};



//from http://teragonaudio.com/article/How-to-make-your-own-VST-host.html

typedef AEffect * (*PFNBRIDGEMAIN)( audioMasterCallback audiomaster, char * pszPluginPath );

// Plugin's entry point
typedef AEffect* (*vstPluginFuncPtr)(audioMasterCallback host);
// Plugin's dispatcher function

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
