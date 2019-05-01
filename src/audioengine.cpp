#include "src/audioengine.h"
#include "SDK/portaudio.h"
#include <src/mainwindow.h>

PaStream *stream;
int AudioEngine::requestedPlaybackPos;

AudioEngine::AudioEngine(MasterTrack *mTrack) : masterTrack(mTrack)
{
}
/*
    Boilerplate PA code
*/
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
    AudioEngine::silenceChannel(output,num_outputs,g_blocksize);
    AudioEngine::silenceChannel(output_storage,num_outputs,g_blocksize);
}
/*
    Boilerplate PA code
*/
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

    input = (float**)malloc(sizeof(float**) * num_outputs);
    output = (float**)malloc(sizeof(float**) * num_outputs);
    for(int channel = 0; channel < num_outputs; channel++)
    {
        input[channel] = (float*)malloc(sizeof(float*) * g_blocksize);
        output[channel] = (float*)malloc(sizeof(float*) * g_blocksize);
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
*/
void AudioEngine::changeBlockSize(int oldSize, int newSize)
{
    for (int i = 0; i < oldSize; ++i)
    {
        delete(input_storage[i]);
        delete(output_storage[i]);
        delete(output[i]);
        delete(input[i]);
    }
    for (uint i = 0; i < masterTrack->midiTracks().size(); ++i)
    {
        auto plugin = masterTrack->midiTracks().at(i)->plugin();
        if(plugin)
            plugin->setBlockSize(newSize);
    }
    delete(input_storage);
    delete(output_storage);
    delete(output);
    delete(input);
    g_blocksize = newSize;
    initializeIO();
}
/*
    Brings each track to it's beginning
    TODO: fn not needed, can be don in changePlaybackPos
*/
void AudioEngine::requestPlaybackRestart()
{
     masterTrack->updateTrackPositions(false,true,-1);
    for (uint i = 0; i < masterTrack->midiTracks().size() ; ++i)
    {
        auto plugin =  masterTrack->midiTracks().at(i)->masterPlugin();
        if(plugin)
        {
            plugin->restartPlayback();
           // masterTrack->midiTracks().at(i)->pianoRoll()->

        }
    }
    _paused = firstPause = false;
}
/*
    Toggles between paused and resumed playback
*/
void AudioEngine::setPaused(bool paused)
{
    _paused = paused;
    firstPause = true;
    masterTrack->updateTrackPositions(paused,false,-1);
}
/*
    Changes where tracks are playing from based on time in ticks
    TODO: Find way to provide ticks that doesn't require global var
*/
void AudioEngine::changePlaybackPos()
{
    int numPlugs = masterTrack->midiTracks().size();
    for (int i = 0; i < numPlugs ; ++i)
    {
        auto plugin =  masterTrack->midiTracks().at(i)->masterPlugin();
        if (plugin == nullptr)  continue;
        plugin->setCustomPlackbackPos(requestedPlaybackPos);
          masterTrack->updateTrackPositions(false,false,requestedPlaybackPos);
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

        engine->masterTrack->unsafeRemoveTrack(track);
        //  track->plugin().unloadPlugin();
    }
    if (AudioEngine::requestedPlaybackPos != -1)
    {
        AudioEngine *audio = static_cast<AudioEngine*>( userData );
        audio->changePlaybackPos();
        customPlayback = true;
    }
    for (uint i = 0; i < engine->masterTrack->midiTracks().size() ; ++i)
    {
        auto track = engine->masterTrack->midiTracks().at(i).get();
        auto plugin = track->plugin();
        if (plugin == nullptr) continue;
        if (!plugin->canProcess()) continue;

        if (engine->isPaused() )
        {
            if(engine->firstPause)
            {
                engine->firstPause = false;
                plugin->turnOffAllNotes();
            }
            else {
               plugin->processMidi(true);
            }

            plugin->processAudio(engine->input,engine->output,g_blocksize,true);
        }
        else
        {
            if (customPlayback)
            {
                //TODO
            }
            plugin->processMidi(false);
            plugin->processAudio(engine->input,engine->output,g_blocksize,false);
        }
        if(track->isMuted())
        {
            AudioEngine::silenceChannel(engine->output,engine->num_outputs,g_blocksize);
        }
        else
        {

            for (int i = 0; i < g_blocksize; ++i)
            {
                engine->output_storage[0][i] += engine->output[0][i] * g_volume ;
                engine->output_storage[1][i] += engine->output[1][i] * g_volume ;

            }

            AudioEngine::silenceChannel(engine->output,engine->num_outputs,g_blocksize);
        }
    }
    if (customPlayback)
    {
        AudioEngine::requestedPlaybackPos = -1;
    }

    float *out = static_cast<float*> (outputBuffer);
    for(uint i=0; i<framesPerBuffer; i++ )
    {
        *out++ = engine->output_storage[0][i];  /* left */
        *out++ = engine->output_storage[1][i];  /* right */
    }

    AudioEngine::silenceChannel(engine->output_storage,engine->num_outputs,g_blocksize);
    return 0;
}

