#include "src/audioengine.h"
#include "SDK/portaudio.h"
#include <src/mainwindow.h>
PaStream *stream;

int AudioEngine::requestedPlaybackPos;
bool AudioEngine::shouldDeleteTrack = false;
bool isPaused = false;


float** inputss = 0;
float** outputss = 0;

float** input_storage= 0;
float** output_storage = 0;
int num_outputs = 64;
AudioEngine::AudioEngine(MasterTrack *mTrack) : masterTrack(mTrack)
{

}

void AudioEngine::startPortAudio()
{
    PaError err = Pa_Initialize();
    if( err != paNoError )
    {
        qDebug() << (  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
        return;
    }
    isRunning = true;
    initializeIO();
    AudioEngine::silenceChannel(outputss,num_outputs,g_blocksize);
    AudioEngine::silenceChannel(output_storage,num_outputs,g_blocksize);
}

void AudioEngine::stopPortAudio()
{
    PaError err = Pa_CloseStream(stream);
    if( err != paNoError )
    {
        qDebug() << (  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
        return;
    }
    err = Pa_Terminate();
    if( err != paNoError )
    {
        qDebug() << (  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
        return;
    }
}
/*
    Boilerplate PA code
*/
void AudioEngine::openStream()
{

    int numDevices;
    numDevices = Pa_GetDeviceCount();
    qDebug() << numDevices;

    /* Open an audio I/O stream. */
    PaError  err = Pa_OpenDefaultStream( &stream,
                                         0,          /* no input channels */
                                         2,          /* stereo output */
                                         paFloat32,  /* 32 bit floating point output */
                                         sampleRate,
                                         g_blocksize,        // frames per buffer
                                       &patestCallback, /* this is your callback function */
                                       this ); /*This is a pointer that will be passed to
               your callback*/
    if( err != paNoError ) {
        qDebug() << (  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    }
}

/*
    Boilerplate PA code
*/
void AudioEngine::initializeIO() {

    inputss = (float**)malloc(sizeof(float**) * num_outputs);
    outputss = (float**)malloc(sizeof(float**) * num_outputs);
    for(int channel = 0; channel < num_outputs; channel++)
    {
        inputss[channel] = (float*)malloc(sizeof(float*) * g_blocksize);
        outputss[channel] = (float*)malloc(sizeof(float*) * g_blocksize);
    }

    input_storage = (float**)malloc(sizeof(float**) * num_outputs);
    output_storage = (float**)malloc(sizeof(float**) * num_outputs);
    for(int channel = 0; channel < num_outputs; channel++)
    {
        input_storage[channel] = (float*)malloc(sizeof(float*) * g_blocksize);
        output_storage[channel] = (float*)malloc(sizeof(float*) * g_blocksize);
    }
}
/*
    Must end audio engine before calling and start again after
    blocksize is provided through common variable g_blockSize
*/
void AudioEngine::changeBlockSize(int oldSize, int newSize)
{
    for (int i = 0; i < oldSize; ++i)
    {
        delete(input_storage[i]);
        delete(output_storage[i]);
        delete(outputss[i]);
        delete(inputss[i]);
    }
    for (int i = 0; i < masterTrack->midiTracks.size(); ++i)
    {
        auto plugin = masterTrack->midiTracks.at(i)->masterPlugin();
        plugin->setBlockSize(plugin->effect,newSize);
    }
    delete(input_storage);
    delete(output_storage);
    delete(outputss);
    delete(inputss);
    g_blocksize = newSize;
    initializeIO();
}
/*
    Brings each track to it's beginning
    TODO: fn not needed, can be don in changePlaybackPos
*/
void AudioEngine::requestPlaybackRestart()
{
    for (int i = 0; i < masterTrack->midiTracks.size() ; ++i)
    {
        auto plugin=  masterTrack->midiTracks.at(i)->masterPlugin();
        plugin->restartPlayback();
    }
}
/*
    Toggles between paused and resumed playback
*/
void AudioEngine::setPaused(bool isResume)
{
    for (int i = 0; i < masterTrack->midiTracks.size(); ++i)
    {
        auto plugin =  masterTrack->midiTracks.at(i)->masterPlugin();
        if(isResume){
            plugin->pauseOrResumePlayback(true);
            isPaused = false;
        }
        else
        {
            plugin->pauseOrResumePlayback(false);
            isPaused = true;
        }
    }
}
/*
    Changes where tracks are playing from based on time in ticks
    TODO: Find way to provide ticks that doesn't require global var
*/
void AudioEngine::changePlaybackPos()
{
    int numPlugs = masterTrack->midiTracks.size();
    for (int i = 0; i < numPlugs ; ++i)
    {
        auto plugin =  masterTrack->midiTracks.at(i)->masterPlugin();
        if (plugin->effect == NULL)  continue;

        QObject::connect(this,&AudioEngine::changePlaybackPosSignal,plugin,&Vst2HostCallback::setCustomPlackbackPos);
        emit changePlaybackPosSignal(requestedPlaybackPos);
    }
}
/*
    Boilerplate PA code
*/
void AudioEngine::silenceChannel(float **channelData, int numChannels, long numFrames) {
    for(int channel = 0; channel < numChannels; ++channel) {
        for(long frame = 0; frame < numFrames; ++frame) {
            channelData[channel][frame] = 0.0f;
        }
    }
}
/*
    Boilerplate PA code
*/
void AudioEngine::startStream()
{
    PaError err = Pa_StartStream( stream );
    if( err != paNoError )
    {
        qDebug() << (  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    }
}

/*
    The callback used to fill buffers from plugins
*/
int patestCallback( const void *inputBuffer, void *outputBuffer,
                    unsigned long framesPerBuffer,
                   const PaStreamCallbackTimeInfo* timeInfo,
                   PaStreamCallbackFlags statusFlags,
                   void *userData )
{
    auto engine = static_cast<AudioEngine*>(userData);
    bool customPlayback = false;
    while(!engine->masterTrack->tracksToRemove().empty())
    {
        auto track = engine->masterTrack->tracksToRemove().dequeue();
        engine->masterTrack->midiTracks.removeOne(track);
        track->plugin()->unloadPlugin();
        delete track->plugin();

    }
    if (AudioEngine::requestedPlaybackPos != -1)
    {
        AudioEngine *audio = (AudioEngine*)userData;
        audio->changePlaybackPos();
        customPlayback = true;
    }
    for (int i = 0; i < engine->masterTrack->midiTracks.size() ; ++i)
    {

        auto plugin = engine->masterTrack->midiTracks.at(i)->masterPlugin();
        if (plugin->effect == NULL) continue;
        if (!plugin->canPlay) continue;

        if (isPaused)
        {
            plugin->turnOffAllNotes();
            plugin->processAudio(plugin->effect,inputss,outputss,g_blocksize);
        }
        else
        {
            if (customPlayback)
            {
                //TODO
            }
            plugin->processMidi(plugin->effect);
            plugin->processAudio(plugin->effect,inputss,outputss,g_blocksize);
        }
        if(plugin->isMuted)
        {
            AudioEngine::silenceChannel(outputss,num_outputs,g_blocksize);
        }
        else
        {

            for (int i = 0; i < g_blocksize; ++i)
            {
                output_storage[0][i] += outputss[0][i] * g_volume ;
                output_storage[1][i] += outputss[1][i] * g_volume ;

            }

            AudioEngine::silenceChannel(outputss,num_outputs,g_blocksize);
        }
    }
    if (customPlayback)
    {
        AudioEngine::requestedPlaybackPos = -1;
    }
    isPaused = false;

    float *out = (float*)outputBuffer;
    for(uint i=0; i<framesPerBuffer; i++ )
    {
        *out++ =output_storage[0][i];  /* left */
        *out++ =output_storage[1][i];  /* right */
    }

    AudioEngine::silenceChannel(output_storage,num_outputs,g_blocksize);
    return 0;
}

