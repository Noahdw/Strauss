#include "vst2audioplugin.h"
#include <Windows.h>
#include "WinUser.h"
#include <qdebug.h>
#include <SDK/audioeffectx.h>
#include <SDK/vstfxstore.h>
#include "src/trackmidi.h"
#include <qdatetime.h>
#include <src/pianoroll.h>
#include <src/audioengine.h>
#include <src/controlchangeoverlay.h>
#include <src/plugintrackview.h>
#include "src/pluginview.h"
#include "common.h"

#pragma comment(lib,"user32.lib")
#pragma comment(lib, "advapi32")
#define PROXY_REGKEY        "Software\\JBridge"

#ifdef _M_X64
#define PROXY_REGVAL        "Proxy64"  //use this for x64 builds
#else
#define PROXY_REGVAL        "Proxy32"  //use this for x86 builds
#endif

LPCSTR APPLICATION_CLASS_NAME = (LPCSTR)"MIDIHOST";

VstTimeInfo vtime;
float sRate;
double sPos = 0;
int processLevel = 2;//kVstProcessLevelUser; //1


Vst2AudioPlugin::Vst2AudioPlugin(TrackMidi* track) : AudioPlugin (track)
{

}

Vst2AudioPlugin::~Vst2AudioPlugin()
{

    unloadPlugin();
    SendMessageA(editor,WM_QUIT,(WPARAM)hinst,0);
    FreeLibrary(hinst);
}



extern "C"{
bool firstRun = true;    int note = 0;
VstIntPtr VSTCALLBACK hostCallback(AEffect *effect, VstInt32 opcode,
                                   VstInt32 index, VstInt32 value, void *ptr, float opt)
{
    if (opcode != 7)
    {
        //  qDebug() << "DEBUG OPCODE ALWAYS CALLED: " << opcode;
    }

    //http://jdmcox.com/PianoRollComposer.cpp for opcodes
    // or http://www.bass.radio42.com/help/html/6aa9c7ce-730a-9dac-1c3f-d362b08158a1.htm
    switch (opcode) {
    case audioMasterVersion:
        return 2400;
    case audioMasterIdle:
        effect->dispatcher(effect, effEditIdle, 0, 0, 0, 0);

        break;
    case audioMasterCurrentId:
        qDebug() << "audioMasterCurrentId" << opcode;
        return 1;
        break;
    case audioMasterProcessEvents:
        qDebug() << "audioMasterProcessEvents" << opcode;
        break;
    case audioMasterGetTime:
    {
        // some help from https://github.com/elieserdejesus/JamTaba/blob/master/src/Common/vst/VstHost.cpp
        vtime.samplePos = sPos;
        vtime.sampleRate = sRate;
        vtime.nanoSeconds = QDateTime::currentDateTime().currentMSecsSinceEpoch() * 1000000.0;
        vtime.tempo = g_tempo;
        vtime.ppqPos = ((vtime.samplePos)/((60.0 / vtime.tempo) * sRate)) + 1;
        vtime.barStartPos = 0;
        vtime.cycleStartPos = 0.0;
        vtime.cycleEndPos = 0.0;
        vtime.timeSigDenominator = 4;
        vtime.timeSigNumerator = 4;
        vtime.smpteOffset  = 0;
        vtime.smpteFrameRate  = 1;
        vtime.samplesToNextClock = 0;
        vtime.flags = 0;
        vtime.flags |= kVstTempoValid;
        vtime.flags |= kVstTimeSigValid;
        vtime.flags |= kVstPpqPosValid;
        vtime.flags |= kVstTransportPlaying;

        // qDebug() << "audioMasterGetTime" << opcode;
        return (VstIntPtr)&vtime;
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
    case audioMasterGetSampleRate:
        return g_sampleRate;
    case audioMasterCanDo:
        return 1; //I can do it all?
    case audioMasterGetLanguage: //38?
        return 1;

    case audioMasterUpdateDisplay: //42
        if (!firstRun) {
            effect->dispatcher(effect, effEditIdle, 0, 0, NULL, 0.0f);
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

bool Vst2AudioPlugin::loadPlugin(QString path, QString name)
{
    pluginName = name;
    auto cName = pluginName.toLocal8Bit().data();
    AEffect *newPlugin = NULL;
    HMODULE modulePtr = LoadLibraryA(path.toLocal8Bit());
    hinst = modulePtr;
    if(modulePtr == NULL) {
        qDebug() << "failed loading VST: " << GetLastError();
        return false;
    }
    vstPluginFuncPtr mainEntryPoint;
    if (GetProcAddress(modulePtr, "JBridgeBootstrap"))
    {
        qDebug() << "Jbridge plugin";
        FreeLibrary(modulePtr);
        newPlugin = LoadBridgedPlugin(path.toLocal8Bit().data());
        if (newPlugin == NULL)
        {
            qDebug() << "Jbridge plugin failed to load";
            return false;
        }
    }
    else
    {
        mainEntryPoint = (vstPluginFuncPtr)GetProcAddress(modulePtr, "VSTPluginMain");

        if(mainEntryPoint == NULL)
        {
            // mainEntryPoint = (vstPluginFuncPtr)GetProcAddress(modulePtr, "JBridgeBootstrap");

            mainEntryPoint = (vstPluginFuncPtr)GetProcAddress(modulePtr, "main");
            if (mainEntryPoint == NULL) {
                qDebug() << "Could not get main entry point.";
                FreeLibrary(modulePtr);
                return false;
            }
        }
        newPlugin = mainEntryPoint((audioMasterCallback)hostCallback);
    }
    // Instantiate the plugin

    sRate = sampleRate;
    samplesPerTick = sampleRate/(float)(MidiManager::TPQN * (g_tempo/60.0f));
    effect = newPlugin;
    int state = configurePluginCallbacks();
    if (state == -1)
    {
        qDebug() << "Failed to configurePluginCallbacks. abort startPlugin";
        //TODO unload if fails
        return false;
    }
    startPlugin();
    isMasterPlugin = true; //deprecated?
    return true;
}

int Vst2AudioPlugin::configurePluginCallbacks() {
    // Check plugin's magic number
    // If incorrect, then the file either was not loaded properly, is not a
    // real VST plugin, or is otherwise corrupt.
    if(effect->magic != kEffectMagic) {
        qDebug() << "Plugin's magic number is bad";
        return -1;
    }

    // Create dispatcher handle
    dispatcher = (dispatcherFuncPtr)(effect->dispatcher);

    // Set up plugin callback functions
    effect->getParameter = (getParameterFuncPtr)effect->getParameter;
    effect->processReplacing = (processFuncPtr)effect->processReplacing;
    effect->setParameter = (setParameterFuncPtr)effect->setParameter;


    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_CLOSE:
        ShowWindow(hWnd,SW_HIDE);
        return true;
    case WM_QUIT:
        DestroyWindow(hWnd);
        if (!UnregisterClassA(APPLICATION_CLASS_NAME,(HINSTANCE)wParam))
        {
            qDebug() << "Could not unregister class: " << GetLastError();
        }

    }
    return DefWindowProcA(hWnd, msg, wParam, lParam);
}


void Vst2AudioPlugin::startPlugin() {

    dispatcher(effect, effOpen, 0, 0, NULL, 0.0f);
    dispatcher(effect, effSetSampleRate, 0, 0, NULL, sampleRate);
    dispatcher(effect, effSetBlockSize, 0, g_blocksize, NULL, 0.0f);
    dispatcher(effect, effMainsChanged, 0, 1, NULL, 0.0f);

    //This crap taken from https://github.com/falkTX/dssi-vst/blob/master/dssi-vst-server.cpp
    WNDCLASSEXA wclass = { };
    wclass.cbSize = sizeof(WNDCLASSEXA);
    wclass.style = 0;
    wclass.lpfnWndProc = WndProc;
    wclass.cbClsExtra = 0;
    wclass.cbWndExtra = 0;
    wclass.hInstance = hinst;
    wclass.hIcon = LoadIcon(hinst, (LPCWSTR)APPLICATION_CLASS_NAME);
    wclass.hCursor = LoadCursor(0, IDC_ARROW);
    wclass.lpszMenuName = (LPCSTR)"MIDIINTER_HOST";
    wclass.lpszClassName = APPLICATION_CLASS_NAME;
    wclass.hIconSm = 0;
    wclass.hbrBackground =0;

    int err =  RegisterClassExA(const_cast<const WNDCLASSEXA*>(&wclass));
    if (err == 0) {
        qDebug() <<  "Error - Could not register class: " << GetLastError();
        return;
    }

    editor = CreateWindowExA(0,APPLICATION_CLASS_NAME,pluginName.toLocal8Bit(),WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
                             CW_USEDEFAULT, CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,hinst,NULL);
    if (editor == nullptr) {
        qDebug() <<  "Error - Could not create window: " << GetLastError();
        return;
    }
    dispatcher(effect,effEditOpen,0,0,editor,0);
    Rect *rect = 0;
    effect->dispatcher(effect, effEditGetRect, 0, 0, &rect, 0);
    if (!rect)
    {
        unloadPlugin();
        qDebug() << "Error - Could not get plugin rect";
        return;
    }
    SetWindowPos(editor, 0, 0, 0,
                 rect->right - rect->left + 6,
                 rect->bottom - rect->top + 25,
                 SWP_NOMOVE);
    ShowWindow(editor,SW_SHOWNORMAL);
    UpdateWindow(editor);

    initializeMidiEvents();
    setCanProcess (true);
}

void Vst2AudioPlugin::silenceChannel(float **channelData, int numChannels, long numFrames)
{
    for(int channel = 0; channel < numChannels; ++channel) {
        for(long frame = 0; frame < numFrames; ++frame) {
            channelData[channel][frame] = 0.0f;
        }
    }
}
/*
This is used to convert midi arrays into blocks for processing
*/
void Vst2AudioPlugin::processMidi()
{ 
    events->numEvents =0;
    uint i = 0;
    int pos = 0;
    bool canSkip = false;
    int df =0;
    int velocity;
    // These are events from the keyboard to the left of the pianoRoll. in the future this might be
    // merged into the queue below.
    if (eventToAdd.hasEventToAdd) {

        (eventToAdd.eventOn) ? velocity = 70 : velocity = 0;
        VstMidiEvent* evnt = eventsHolder[pos];
        evnt->byteSize =24;
        evnt->deltaFrames = 0;
        evnt->type = kVstMidiType;
        evnt->flags = 0;
        evnt->detune = 0;
        evnt->noteOffVelocity = 0;
        evnt->reserved1 = 0;
        evnt->reserved2 = 0;
        evnt->midiData[0] =  0x90;
        evnt->midiData[1] = (uchar)eventToAdd.note;
        evnt->midiData[2] = (uchar)velocity;
        evnt->midiData[3] = 0;
        evnt->noteOffset = 0;
        evnt->noteLength = 0;

        events->events[pos] = (VstEvent*)evnt;
        ++events->numEvents;
        ++pos;

        eventToAdd.hasEventToAdd = false;
    }
    //These are events which are the result of playback from some midi device (e.g. human genorated from a keyboard).
    while (!midiEventQueue.empty())
    {
        EventToAdd eventStruct = midiEventQueue.front();
        midiEventQueue.pop();
        VstMidiEvent* evnt = eventsHolder[pos];
        int df = 0;
        if (!midiEventQueue.empty())
        {
            midiEventQueue.front().timeInTicks -= eventStruct.timeInTicks;
            midiEventQueue.front().timeInTicks *= samplesPerTick;
        }
        df = eventStruct.timeInTicks;
        evnt->byteSize        = 24;
        evnt->deltaFrames     = df;
        evnt->type            = kVstMidiType;
        evnt->flags           = 0;
        evnt->detune          = 0;
        evnt->noteOffVelocity = 0;
        evnt->reserved1       = 0;
        evnt->reserved2       = 0;
        evnt->midiData[0]     = eventStruct.status;
        evnt->midiData[1]     = eventStruct.note;
        evnt->midiData[2]     = eventStruct.velocity;
        evnt->midiData[3]     = 0;
        evnt->noteOffset      = 0;
        evnt->noteLength      = 0;

        events->events[pos] = (VstEvent*)evnt;
        ++events->numEvents;
        ++pos;

    }


    // If we are too many samples away from inputing an event, subtract blocksize and give empty vstevents
    /* Some help to make sense of this: We want to send our midi events to our plugins but we must abide
     * by the fact that our audio engine calls this function SampleRate/blocksize times every second.
     * e.g. 48000/256 = 187 times per second. Our midi data is stored in a vector by its delta ticks since the last
     * midi note. If the TicksPerQuarterNote is 980 and the BPM is 120, then there are 1960 ticks per second. This tells us that
     * each tick corresponds to 22.9 samples of audio. A single quarter note that starts at the very beginning of a song will have
     * its note-off a DT of 960 from its note-on. The note-on is added instantly as an event as it has a DT of 0, but the note-off
     * is 960 * 22.9 (21984) samples away. We can subract the block size from this every time the function is called until we are in
     * the range [0,block size] at which point it is added as an event. Delta frames are the samples since the start of a block,
     * or in our case the value our FramesTillBLock is at when it is in the range to add an event. Delta frames only really matter when
     * the block size is quite large.
     *
     * This might all be 100% wrong :)
     */
    if (!hasReachedEnd) {
        auto cc = midiTrack()->cc();
        for (const auto& c : cc) //TODO
        {
            int p = c.first;
            df = 0;
            canSkip = false;
            int k = 0;

            while(k < g_blocksize){
                if (ccVecPos[p] >= cc.at(p).listOfNotes.size())
                {
                    break;
                }
                if (ccFramesTillBlock[p] >= 0)
                {
                    if (ccFramesTillBlock[p] < g_blocksize)
                    {
                        canSkip = true;
                    }
                    else
                    {
                        ccFramesTillBlock[p] -= g_blocksize;
                        break;
                    }
                }
                if (!canSkip) {
                    //Deltaframes, bless its heart, is actually samples since the start of a block.
                    // A lot of sources claimed it was Microseconds since the start. Wrong.
                    df = cc.at(p).listOfNotes.at(ccVecPos[p]) * samplesPerTick;
                    k+=df;
                    if (k > g_blocksize) {
                        ccFramesTillBlock[p] = df;
                        break;
                    }
                }
                else
                {
                    df = ccFramesTillBlock[p];
                    k+=df;
                    canSkip = false;
                }

                ccFramesTillBlock[p] = -1;

                //can add new event as we are in block timeframe
                VstMidiEvent* evnt = eventsHolder[pos];
                evnt->byteSize =24;
                evnt->deltaFrames = df;
                evnt->type = kVstMidiType;
                evnt->flags = 0;
                evnt->detune = 0;
                evnt->noteOffVelocity = 0;
                evnt->reserved1 = 0;
                evnt->reserved2 = 0;
                evnt->midiData[0] = (uchar)cc.at(p).listOfNotes.at(ccVecPos[p] + 1) & 0xFF;
                evnt->midiData[1] = (uchar)(cc.at(p).listOfNotes.at(ccVecPos[p] + 1) >> 8) & 0xFF;
                evnt->midiData[2] = (uchar)(cc.at(p).listOfNotes.at(ccVecPos[p] + 1) >> 16) & 0xFF;
                evnt->midiData[3] = 0;
                evnt->noteOffset = 0;
                evnt->noteLength = 0;
                events->events[pos] = (VstEvent*)evnt;

                ++pos;
                ++events->numEvents;
                ccVecPos[p] += 2;
            }

        }

        while(i < g_blocksize){
            if (noteVecPos >= midiTrack()->midiData()->listOfNotes.length()) {
                qDebug() << "notevec length :" << midiTrack()->midiData()->listOfNotes.length();
                qDebug() << "Reached end of midi";
                hasReachedEnd = true;
                break;
            }
            if (framesTillBlock >= 0) {
                if (framesTillBlock < g_blocksize) {
                    canSkip = true;
                }
                else
                {
                    framesTillBlock -= g_blocksize;
                    break;
                }
            }
            if (!canSkip) {
                //Deltaframes, bless its heart, is actually samples since the start of a block.
                // A lot of sources claimed it was Microseconds since the start. Wrong.
                df = midiTrack()->midiData()->listOfNotes.at(noteVecPos) * samplesPerTick;
                i+=df;
                if (i > g_blocksize) {
                    framesTillBlock = df;
                    break;
                }
            }
            else
            {
                df = framesTillBlock;
                i+=df;
                canSkip = false;
            }

            framesTillBlock = -1;

            //can add new event as we are in block timeframe
            VstMidiEvent* evnt = eventsHolder[pos];
            evnt->byteSize =24;
            evnt->deltaFrames = df;
            evnt->type = kVstMidiType;
            evnt->flags = 0;
            evnt->detune = 0;
            evnt->noteOffVelocity = 0;
            evnt->reserved1 = 0;
            evnt->reserved2 = 0;
            evnt->midiData[0] = (uchar)midiTrack()->midiData()->listOfNotes.at(noteVecPos+1) & 0xFF;
            evnt->midiData[1] = (uchar)(midiTrack()->midiData()->listOfNotes.at(noteVecPos+1) >>8) & 0xFF;
            evnt->midiData[2] = (uchar)(midiTrack()->midiData()->listOfNotes.at(noteVecPos+1) >>16) & 0xFF;
            evnt->midiData[3] = 0;
            evnt->noteOffset = 0;
            evnt->noteLength = 0;

            events->events[pos] = (VstEvent*)evnt;

            ++pos;
            ++events->numEvents;
            noteVecPos +=2;
        }
    }
    dispatcher(effect, effProcessEvents, 0, 0, events, 0.0f);
    sPos += g_blocksize;

}
//There is no need to malloc every call and create new events. Reuse just one structure.
void Vst2AudioPlugin::initializeMidiEvents()
{
    events = (VstEvents*)malloc(sizeof(VstEvents) + sizeof(VstEvents*)*(maxNotes));
    for (uint i = 0; i < maxNotes; ++i)
    {
        VstMidiEvent* evnt = new VstMidiEvent;
        eventsHolder[i] = evnt;
    }
    //  events->numEvents = maxNotes;
    events->reserved = 0;
    qDebug() << "initializeMidiEvents okay";
}

void Vst2AudioPlugin::restartPlayback()
{
    for (int i = 0; i < 128; ++i)
    {
        ccFramesTillBlock[i] = -1;
        ccVecPos[i] = 0;
    }
    noteVecPos = 0;
    framesTillBlock = -1;
    hasReachedEnd = false;
}



void Vst2AudioPlugin::addMidiEvent(uchar status,uchar note, uchar velocity, qreal currentTick)
{
    EventToAdd evnt{status,note,false,false,currentTick,velocity};
    midiEventQueue.push(evnt);
}

void Vst2AudioPlugin::setCustomPlackbackPos(int playbackPos)
{
    int total = 0;
    for (int var = 0; var < midiTrack()->midiData()->listOfNotes.length(); var+= 2)
    {
        total += midiTrack()->midiData()->listOfNotes.at(var);
        if (total >= playbackPos)
        {
            noteVecPos = var;
            framesTillBlock = -1;
            hasReachedEnd = false;
            return;
        }
    }
}

void Vst2AudioPlugin::setPianoRollRef(PianoRoll * piano)
{
    pianoroll = piano;
}



void Vst2AudioPlugin::turnOffAllNotes()
{
    if(effect == nullptr) return;
    events->numEvents = 0;
    for (int i = 0; i < 128; ++i)
    {
        VstMidiEvent* evnt = eventsHolder[i];
        evnt->byteSize =24;
        evnt->deltaFrames = 0;
        evnt->type = kVstMidiType;
        evnt->flags = 0;
        evnt->detune = 0;
        evnt->noteOffVelocity = 0;
        evnt->reserved1 = 0;
        evnt->reserved2 = 0;
        evnt->midiData[0] =  0x90;
        evnt->midiData[1] = (uchar)i;
        evnt->midiData[2] = 0;
        evnt->midiData[3] = 0;
        evnt->noteOffset = 0;
        evnt->noteLength = 0;
        events->events[i] = reinterpret_cast<VstEvent*>(evnt);
        ++events->numEvents;
    }
    dispatcher(effect, effProcessEvents, 0, 0, events, 0.0f);
}

void Vst2AudioPlugin::showPlugin()
{
    if (editor == nullptr) return;

    ShowWindow(editor,SW_RESTORE);
    SetForegroundWindow(editor);

}

void Vst2AudioPlugin::hidePlugin()
{
    if (editor == nullptr) return;

    ShowWindow(editor,SW_HIDE);
}

void Vst2AudioPlugin::exportAudioInit()
{
    for (int i = 0; i < 128; ++i)
    {
        ccFramesTillBlock[i] = -1;
        ccVecPos[i] = 0;
    }
    noteVecPos = 0;
    framesTillBlock = -1;
    hasReachedEnd = false;
    processLevel = kVstProcessLevelOffline;
}



int Vst2AudioPlugin::exportAudioBegin(float **outputs,
                                      int numFrames)
{
     if (editor == nullptr) return 0;
    processMidi();
    processAudio(outputs,outputs,numFrames);
    if (hasReachedEnd)
    {
        return 0;
    }
    return 1;
}

void Vst2AudioPlugin::unloadPlugin()
{
    if(effect == nullptr) return;
    dispatcher(effect,effEditClose,0,0,NULL,0);
    dispatcher(effect,effMainsChanged,0, 0, NULL, 0);
    dispatcher(effect,effClose, 0, 0, NULL, 0.0f);
}

void Vst2AudioPlugin::exportAudioEnd()
{
    for (int i = 0; i < 128; ++i)
    {
        ccFramesTillBlock[i] = -1;
        ccVecPos[i] = 0;
    }
    noteVecPos = 0;
    framesTillBlock = -1;
    hasReachedEnd = false;
    processLevel = kVstProcessLevelRealtime;
}

std::string Vst2AudioPlugin::savePluginState() const
{
    if (effect && (effect->flags & effFlagsProgramChunks))
    {
        void *data;
        auto dataSize = dispatcher(effect,effGetChunk,0,0,&data,0.0);
        if (dataSize == 0 || data == nullptr)
        {
            return "";
        }
        std::string strData((char*)data,dataSize);
        return strData;

    }
    return "";
}

void Vst2AudioPlugin::setPluginState(const std::string &chunk)
{
    if (effect == nullptr || chunk.length() == 0) return;

    dispatcher(effect,effSetChunk,0,chunk.length(),(void*)&chunk.c_str()[0],0);

}

void Vst2AudioPlugin::processAudio(float **inputs, float **outputs,
                                   int numFrames)
{
    if (!canProcess()) return;

    samplesPerTick = sampleRate/(float)(MidiManager::TPQN * (g_tempo/60.0f));
    if (effect->flags & effFlagsCanReplacing)
    {
        effect->processReplacing(effect, inputs, outputs, numFrames);
        if (!isMasterPlugin)
            return;



        for (int i = 0; i < g_blocksize; ++i)
        {

            outputs[0][i] *=  0.5 - pan;
            outputs[1][i] *=  0.5 + pan;

            outputs[0][i] *= trackVolume;
            outputs[1][i] *= trackVolume;

        }
        for (const auto &p : midiTrack()->effectPlugins)
        {
            if (p->canProcess())
            {
                p->processAudio(outputs,outputs,numFrames);
            }
        }
    }
    else
    {
        qDebug() << "processReplacing not supported by plugin";
    }
}

void Vst2AudioPlugin::setBlockSize(int blockSize)
{
    if(effect == nullptr) return;
    dispatcher(effect, effSetBlockSize, 0, blockSize, NULL, 0.0f);
}



AEffect* Vst2AudioPlugin::LoadBridgedPlugin(char * szPath)
{
    // Get path to JBridge proxy
    char szProxyPath[1024];
    szProxyPath[0] = 0;
    HKEY hKey;

    if( RegOpenKeyA(HKEY_LOCAL_MACHINE, PROXY_REGKEY, &hKey) == ERROR_SUCCESS )
    {
        DWORD dw = sizeof(szProxyPath);
        RegQueryValueExA(hKey, PROXY_REGVAL, NULL, NULL, (LPBYTE)szProxyPath, &dw);
        RegCloseKey(hKey);
    }

    // Check key found and file exists
    if (szProxyPath[0] == 0)
    {
        qDebug() << "Unable to locate proxy DLL";
        return nullptr;
    }


    // Load proxy DLL
    HMODULE hModuleProxy = LoadLibraryA(szProxyPath);
    if (!hModuleProxy)
    {
        qDebug() << "Failed to load proxy DLL";
        return nullptr;
    }

    // Get entry point
    PFNBRIDGEMAIN pfnBridgeMain = (PFNBRIDGEMAIN)GetProcAddress(hModuleProxy, "BridgeMain");
    if (!pfnBridgeMain)
    {
        FreeLibrary(hModuleProxy);
        qDebug() << "Failed to get proxy entry point";
        return nullptr;
    }
    hinst = hModuleProxy;
    return pfnBridgeMain((audioMasterCallback)hostCallback,szPath);  
}

int Vst2AudioPlugin::numParams(AEffect *plugin) const
{
    if (plugin)
    {
        return plugin->numParams;
    }
    return -1;
}

QString Vst2AudioPlugin::getParamName(AEffect *plugin,int index)
{
    char n[100];
    dispatcher(plugin,effGetParamName,index,0,n,0.0f);
    return QString(n);
}
