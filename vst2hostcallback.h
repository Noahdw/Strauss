#ifndef VST2HOSTCALLBACK_H
#define VST2HOSTCALLBACK_H
#include "SDK/aeffectx.h"
#include <SDK/aeffect.h>
#include <SDK/vstfxstore.h>

class Vst2HostCallback
{

public:
    Vst2HostCallback();
    AEffect* loadPlugin(char* fileName);
    int configurePluginCallbacks(AEffect *plugin);
    void startPlugin(AEffect *plugin);
    void initializeIO();
    void processAudio(AEffect *plugin, float **inputs, float **outputs,
      long numFrames);
    void silenceChannel(float **channelData, int numChannels, long numFrames);
    void processMidi(AEffect *plugin, VstEvents *events);
    
    unsigned int blocksize = 512;
    float sampleRate = 44100.0f;
private:


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
