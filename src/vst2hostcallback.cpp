#include "vst2hostcallback.h"
#include <Windows.h>
#include <qdebug.h>
#include <SDK/audioeffectx.h>
#include <qdatetime.h>
#include <src/pianoroll.h>
#pragma comment(lib,"user32.lib")

Vst2HostCallback::Vst2HostCallback(mTrack *mtrack)
{
    track = mtrack;

}
dispatcherFuncPtr dispatcher;
VstTimeInfo vtime;
float sRate;
double sPos = 0;
int processLevel = kVstProcessLevelUser; //1

extern "C"{
bool firstRun = true;
VstIntPtr VSTCALLBACK hostCallback(AEffect *effect, VstInt32 opcode,
                                   VstInt32 index, VstInt32 value, void *ptr, float opt)
{
    // qDebug() << "DEBUG OPCODE ALWAYS CALLED: " << opcode;
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
        // some help from https://github.com/elieserdejesus/JamTaba/blob/master/src/Common/vst/VstHost.cpp

        vtime.samplePos = sPos;
        vtime.sampleRate = sRate;
        vtime.nanoSeconds = QDateTime::currentDateTime().currentMSecsSinceEpoch() * 1000000.0;
        vtime.tempo = 120;
        vtime.ppqPos = ((vtime.samplePos)/((60.0 / vtime.tempo) * sRate)) + 1;
        // qDebug() << time.ppqPos;
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
    // samplesPerTick =(BPM / TPQN )/(1000000/sRate);/
    samplesPerTick =(60.0/120.0 )*(1000000/sRate);//Why do i need /2 to work????
    qDebug() << "SamplesperTick: " << samplesPerTick;
    return plugin;
}

int Vst2HostCallback::configurePluginCallbacks(AEffect *plugin) {
    // Check plugin's magic number
    // If incorrect, then the file either was not loaded properly, is not a
    // real VST plugin, or is otherwise corrupt.
    if(plugin->magic != kEffectMagic) {
        qDebug() << "Plugin's magic number is bad";
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_CLOSE:

        return true;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Vst2HostCallback::startPlugin(AEffect *plugin) {

    dispatcher(plugin, effOpen, 0, 0, NULL, 0.0f);
    dispatcher(plugin, effSetSampleRate, 0, 0, NULL, sampleRate);
    dispatcher(plugin, effSetBlockSize, 0, blocksize, NULL, 0.0f);
    dispatcher(plugin, effMainsChanged, 0, 1, NULL, 0.0f);

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

    HWND editor;
    editor = CreateWindowExA(0,APPLICATION_CLASS_NAME,"mywindow",WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,hinst,NULL);
    if (editor == NULL) {
        qDebug() <<  "Error - Could not create window: " << GetLastError();
        return;
    }
    dispatcher(plugin,effEditOpen,0,0,editor,0);
    ShowWindow(editor,SW_SHOW);
    UpdateWindow(editor);

    initializeMidiEvents();
    canPlay = true;
}

void Vst2HostCallback::silenceChannel(float **channelData, int numChannels, long numFrames) {
    for(int channel = 0; channel < numChannels; ++channel) {
        for(long frame = 0; frame < numFrames; ++frame) {
            channelData[channel][frame] = 0.0f;
        }
    }
}
/*
This is used to convert midi arrays into blocks for processing
*/
void Vst2HostCallback::processMidi(AEffect *plugin)
{ 
    processLevel = kVstProcessLevelRealtime;

    events->numEvents =0;
    //  qDebug() << track->listOfNotes.length();
    uint i = 0;
    int pos = 0;
    bool canSkip = false;
    int df =0;
    int velocity;

    if (eventToAdd.hasEventToAdd) {

        (eventToAdd.eventOn) ? velocity = 70 : velocity = 0;
        VstMidiEvent* evnt = new VstMidiEvent;
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
    while (!midiEventQueue.empty())
    {
        EventToAdd eventStruct = midiEventQueue.front();

        VstMidiEvent* evnt = new VstMidiEvent;
        evnt->byteSize =24;
        evnt->deltaFrames = 0;
        evnt->type = kVstMidiType;
        evnt->flags = 0;
        evnt->detune = 0;
        evnt->noteOffVelocity = 0;
        evnt->reserved1 = 0;
        evnt->reserved2 = 0;
        evnt->midiData[0] =  0x90;
        evnt->midiData[1] = (uchar)eventStruct.note;
        evnt->midiData[2] = eventStruct.velocity;
        evnt->midiData[3] = 0;
        evnt->noteOffset = 0;
        evnt->noteLength = 0;

        events->events[pos] = (VstEvent*)evnt;
        ++events->numEvents;
        ++pos;
         midiEventQueue.pop();
    }

    if (isPaused)
    {
        dispatcher(plugin, effProcessEvents, 0, 0, events, 0.0f);
        return;
    }
    // If we are too many samples away from inputing an event, subtract blocksize and give empty vstevents
    if (!hasReachedEnd) {

        while(i < blocksize){
            //   qDebug() << framesTillBlock;
            if (noteVecPos >= track->listOfNotes.length()) {
                qDebug() << "notevec length :" << track->listOfNotes.length();
                qDebug() << "Reached end of midi";
                hasReachedEnd = true;
                break;
            }
            if (framesTillBlock > 0) {
                if (framesTillBlock < blocksize) {
                    canSkip = true;
                }
                else
                {
                    framesTillBlock -= blocksize;
                    break;
                }
            }
            if (!canSkip) {
                //Deltaframes, bless its heart, is actually samples since the start of a block.
                // A lot of sources claimed it was Microseconds since the start. Wrong.
                df = track->listOfNotes.at(noteVecPos) * samplesPerTick;
                i+=df;
                if (i > blocksize) {
                    framesTillBlock = df - (blocksize - i);
                    break;
                }
            }
            else
            {
                df = framesTillBlock;
                i+=df;
                canSkip = false;
            }

            framesTillBlock = 0;


            //can add new event as we are in block timeframe
            VstMidiEvent* evnt = new VstMidiEvent;

            evnt->byteSize =24;
            evnt->deltaFrames = df;
            evnt->type = kVstMidiType;
            evnt->flags = 0;
            evnt->detune = 0;
            evnt->noteOffVelocity = 0;
            evnt->reserved1 = 0;
            evnt->reserved2 = 0;
            evnt->midiData[0] =  (uchar)track->listOfNotes.at(noteVecPos+2) & 0xFF;
            evnt->midiData[1] = (uchar)(track->listOfNotes.at(noteVecPos+2) >>8) & 0xFF;
            evnt->midiData[2] = (uchar)(track->listOfNotes.at(noteVecPos+2) >>16) & 0xFF;
            evnt->midiData[3] = 0;
            evnt->noteOffset = 0;
            evnt->noteLength = 0;

            events->events[pos] = (VstEvent*)evnt;

            ++pos;
            ++events->numEvents;
            noteVecPos +=3;
        }
    }
    dispatcher(plugin, effProcessEvents, 0, 0, events, 0.0f);
    sPos += blocksize;

}
//There is no need to malloc every call and create new events. Reuse just one structure.
void Vst2HostCallback::initializeMidiEvents()
{
    int numEventsRequired = maxNotes;
    events = (VstEvents*)malloc(sizeof(VstEvents) + sizeof(VstEvents*)*(numEventsRequired));
    events->numEvents = numEventsRequired;
    events->reserved = 0;
    qDebug() << "initializeMidiEvents okay";
}

void Vst2HostCallback::restartPlayback()
{
    noteVecPos = 0;
    framesTillBlock = 0;
    hasReachedEnd = false;
    isPaused = false;
    pianoroll->updateSongTrackerPos(false,false,-1);
}

void Vst2HostCallback::pauseOrResumePlayback(bool isResume)
{
    if (isResume)
    {
        pianoroll->updateSongTrackerPos(true,true,-1);
        isPaused = false;
    }else
    {
        isPaused = true;
        pianoroll->updateSongTrackerPos(true,false,-1);
    }
}

void Vst2HostCallback::addMidiEvent(uchar note, uchar velocity)
{
    EventToAdd evnt{note,false,false,velocity};
    midiEventQueue.push(evnt);
}

void Vst2HostCallback::setCustomPlackbackPos(int playbackPos)
{
    pianoroll->updateSongTrackerPos(false,false,playbackPos);
    int total = 0;
    for (int var = 0; var < track->listOfNotes.length(); var+= 3)
    {
        total += track->listOfNotes.at(var);
        if (total >= playbackPos)
        {
            noteVecPos = var;
            framesTillBlock = 0;
            hasReachedEnd = false;
            isPaused = false;
            return;
        }
    }
}

void Vst2HostCallback::setPianoRollRef(PianoRoll * piano)
{
    pianoroll = piano;
}

void Vst2HostCallback::setCanRecord(bool canRec)
{
    canRecording = canRec;
}

bool Vst2HostCallback::canRecord()
{
    return canRecording;
}

void Vst2HostCallback::turnOffAllNotes(AEffect *plugin)
{
    for (int i = 0; i < 128; ++i)
    {
        VstMidiEvent* evnt = new VstMidiEvent;
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
        events->events[i] = (VstEvent*)evnt;
        ++events->numEvents;
    }
    dispatcher(plugin, effProcessEvents, 0, 0, events, 0.0f);
}


void Vst2HostCallback::processAudio(AEffect *plugin, float **inputs, float **outputs,
                                    long numFrames) {
    // qDebug() << plugin->numInputs;

    if (plugin->flags & effFlagsCanReplacing) {
        plugin->processReplacing(plugin, inputs, outputs, numFrames);
    }
    else{
        qDebug() << "processReplacing not supported by plugin";
    }
}

