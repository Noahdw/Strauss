#include "vst2hostcallback.h"
#include <Windows.h>
#include <qdebug.h>
#include <SDK/audioeffectx.h>

#pragma comment(lib,"user32.lib")

Vst2HostCallback::Vst2HostCallback()
{

}
dispatcherFuncPtr dispatcher;
HMODULE hinst;
float **outputs;
float ** inputs;
uint numChannels = 2;
float sRate;
int processLevel = kVstProcessLevelUser; //1
//static VstIntPtr VSTCALLBACK hostCallback(AEffect *effect, VstInt32 opcode,
// VstInt32 index, VstInt32 value, void *ptr, float opt);


extern "C"{
bool firstRun = true;
VstIntPtr VSTCALLBACK hostCallback(AEffect *effect, VstInt32 opcode,
                                   VstInt32 index, VstInt32 value, void *ptr, float opt)
{
    qDebug() << "DEBUG OPCODE ALWAYS CALLED: " << opcode;
    //http://jdmcox.com/PianoRollComposer.cpp for opcodes
    switch (opcode) {
    case audioMasterVersion:
        return 2400;
    case audioMasterIdle:
        dispatcher(effect, effEditIdle, 0, 0, 0, 0);
        break;
    case audioMasterCurrentId:
        qDebug() << "audioMasterCurrentId" << opcode;
        break;
    case audioMasterProcessEvents:
        qDebug() << "audioMasterProcessEvents" << opcode;
        break;
    case audioMasterGetTime:
    {
        VstTimeInfo *time = new VstTimeInfo();
        time->samplePos = 0;
        time->sampleRate = sRate;
        time->tempo = 120;
        time->ppqPos = 0;
        time->barStartPos = 0;
        time->timeSigDenominator = 4;
        time->timeSigNumerator = 4;


        qDebug() << "audioMasterGetTime" << opcode;
        return (VstIntPtr)time;
    }
    case 6:
        qDebug() << "audioMasterWantMidi" << opcode;
        return 1;


    case audioMasterGetVendorString:
        break;
    case audioMasterGetProductString:
        break;
    case audioMasterGetVendorVersion:
        return 250; //???
    case audioMasterGetCurrentProcessLevel:
        return processLevel;
    case audioMasterVendorSpecific:
        return 1;
    case audioMasterCanDo:
        return 1; //I can do it all?
    case audioMasterUpdateDisplay: //42
        if (!firstRun) {
            dispatcher(effect, effEditIdle, 0, 0, NULL, 0.0f);
        }
        firstRun = false;
        break;
    default:
        qDebug() << "unhandled Plugin opcode requests:" << opcode;
        break;
    }

    VstIntPtr result = 0;
    return result;
}
}
//from http://teragonaudio.com/article/How-to-make-your-own-VST-host.html

AEffect *Vst2HostCallback::loadPlugin(char* fileName)
{
    AEffect *plugin = NULL;


    HMODULE modulePtr = LoadLibraryA(fileName);
    hinst = modulePtr;
    if(modulePtr == NULL) {
        qDebug() << "failed loading VST: " << GetLastError();
        return NULL;
    }

    vstPluginFuncPtr mainEntryPoint =
            (vstPluginFuncPtr)GetProcAddress(modulePtr, "VSTPluginMain");

    if(mainEntryPoint == NULL)
    {
        mainEntryPoint = (vstPluginFuncPtr)GetProcAddress(modulePtr, "main");
        if (mainEntryPoint == NULL) {
            qDebug() << "Could not get main entry point.";
            return NULL;
        }

    }
    // Instantiate the plugin
    plugin = mainEntryPoint((audioMasterCallback)hostCallback);
    sRate = sampleRate;
    return plugin;
}

int Vst2HostCallback::configurePluginCallbacks(AEffect *plugin) {
    // Check plugin's magic number
    // If incorrect, then the file either was not loaded properly, is not a
    // real VST plugin, or is otherwise corrupt.
    if(plugin->magic != kEffectMagic) {
        printf("Plugin's magic number is bad\n");
        return -1;
    }

    // Create dispatcher handle
    dispatcher = (dispatcherFuncPtr)(plugin->dispatcher);

    // Set up plugin callback functions
    plugin->getParameter = (getParameterFuncPtr)plugin->getParameter;
    plugin->processReplacing = (processFuncPtr)plugin->processReplacing;
    plugin->setParameter = (setParameterFuncPtr)plugin->setParameter;


    return (int)plugin;
}

void Vst2HostCallback::startPlugin(AEffect *plugin) {

    dispatcher(plugin, effOpen, 0, 0, NULL, 0.0f);
    dispatcher(plugin, effSetSampleRate, 0, 0, NULL, sampleRate);
    dispatcher(plugin, effSetBlockSize, 0, blocksize, NULL, 0.0f);
    dispatcher(plugin, effMainsChanged, 0, 1, NULL, 0.0f);

    HWND editor;
    editor = CreateWindowExA(WS_OVERLAPPEDWINDOW,"myclass","mywindow",WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,hinst,NULL);
    if (editor == NULL) {
        qDebug() <<  "Could not create window - Error: " << GetLastError();
        return;
    }
    dispatcher(plugin,effEditOpen,0,0,editor,0);
    ShowWindow(editor,SW_SHOW);
    UpdateWindow(editor);
    //initializeIO();

}

void Vst2HostCallback::initializeIO() {
    // inputs and outputs are assumed to be float** and are declared elsewhere,
    // most likely the are fields owned by this class. numChannels and blocksize
    // are also fields, both should be size_t (or unsigned int, if you prefer).
    inputs = (float**)malloc(sizeof(float**) * numChannels);
    outputs = (float**)malloc(sizeof(float**) * numChannels);
    for(int channel = 0; channel < numChannels; channel++) {
        inputs[channel] = (float*)malloc(sizeof(float*) * blocksize);
        outputs[channel] = (float*)malloc(sizeof(float*) * blocksize);
    }
}

void Vst2HostCallback::processAudio(AEffect *plugin, float **inputs, float **outputs,
                                    long numFrames) {
    // Always reset the output array before processing.
    // silenceChannel(outputs, numChannels, numFrames);




    // silenceChannel(inputs, numChannels, numFrames);
    qDebug() << plugin->numInputs;
    // qDebug() << 2;

    plugin->processReplacing(plugin, inputs, outputs, numFrames);
    qDebug() << "processReplacing end";

}

void Vst2HostCallback::silenceChannel(float **channelData, int numChannels, long numFrames) {
    for(int channel = 0; channel < numChannels; ++channel) {
        for(long frame = 0; frame < numFrames; ++frame) {
            channelData[channel][frame] = 0.0f;
        }
    }
}

void Vst2HostCallback::processMidi(AEffect *plugin, VstEvents *events)
{
    processLevel = kVstProcessLevelRealtime;
    dispatcher(plugin, effProcessEvents, 0, 0, events, 0.0f);
    qDebug() << "processReplacing begin";

    if (plugin->flags & effFlagsCanReplacing) {
        // processAudio(plugin,inputs,outputs,1024);
    }
    else{
        qDebug() << "processReplacing not supported by plugin";
    }


}
