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
AudioEngine::AudioEngine()
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
void AudioEngine::initializeIO() {
    // inputs and outputs are assumed to be float** and are declared elsewhere,
    // most likely the are fields owned by this class. numChannels and blocksize
    // are also fields, both should be size_t (or unsigned int, if you prefer).
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
// Must end audio engine before calling and start again after
// blocksize is provided through common variable g_blockSize
void AudioEngine::changeBlockSize(int oldSize, int newSize)
{
    for (int i = 0; i < oldSize; ++i)
    {
        delete(input_storage[i]);
        delete(output_storage[i]);
        delete(outputss[i]);
        delete(inputss[i]);
    }
    for (int i = 0; i < MainWindow::pluginHolderVec.size(); ++i)
    {
        auto holder = MainWindow::pluginHolderVec.at(i);
        holder->host->setBlockSize(holder->effect,newSize);
    }
    delete(input_storage);
    delete(output_storage);
    delete(outputss);
    delete(inputss);
    g_blocksize = newSize;
    initializeIO();
}

void AudioEngine::requestPlaybackRestart()
{
    for (int var = 0; var < MainWindow::pluginHolderVec.length() ; ++var)
    {
        pluginHolder* plugs=  MainWindow::pluginHolderVec.at(var);
        plugs->host->restartPlayback();
    }
}

void AudioEngine::requestPauseOrResume(bool isResume)
{
    for (int var = 0; var < MainWindow::pluginHolderVec.length() ; ++var)
    {
        pluginHolder* plugs=  MainWindow::pluginHolderVec.at(var);
        if(isResume){
            plugs->host->pauseOrResumePlayback(true);
            isPaused = false;
        }
        else
        {
            plugs->host->pauseOrResumePlayback(false);
            //plugs->host->turnOffAllNotes(plugs->effect);
            isPaused = true;
        }
    }
}

void AudioEngine::changePlaybackPos()
{
    int numPlugs = MainWindow::pluginHolderVec.length();
    for (int var = 0; var < numPlugs ; ++var)
    {

        pluginHolder* plugs=  MainWindow::pluginHolderVec.at(var);
        if (plugs->effect ==NULL) {
            //   qDebug() <<"Plugin not set in Audiomanager";
            continue;
        }
        QObject::connect(this,&AudioEngine::changePlaybackPosSignal,plugs->host,&Vst2HostCallback::setCustomPlackbackPos);
        emit changePlaybackPosSignal(requestedPlaybackPos);
    }
}

void AudioEngine::silenceChannel(float **channelData, int numChannels, long numFrames) {
    for(int channel = 0; channel < numChannels; ++channel) {
        for(long frame = 0; frame < numFrames; ++frame) {
            channelData[channel][frame] = 0.0f;
        }
    }
}
void AudioEngine::startStream()
{
    PaError err = Pa_StartStream( stream );
    if( err != paNoError )
    {
        qDebug() << (  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    }
   // requestPlaybackRestart();
}

int num = 0;

int patestCallback( const void *inputBuffer, void *outputBuffer,
                    unsigned long framesPerBuffer,
                    const PaStreamCallbackTimeInfo* timeInfo,
                    PaStreamCallbackFlags statusFlags,
                    void *userData )
{
    bool customPlayback = false;
    float *out = (float*)outputBuffer;
    unsigned int i;
    int numPlugs = MainWindow::pluginHolderVec.length();
    (void) inputBuffer; /* Prevent unused variable warning. */
    if (AudioEngine::shouldDeleteTrack)
    {
        qDebug() <<"I was called";
        for (int var = 0; var < numPlugs ; ++var)
        {
            pluginHolder *plugs = MainWindow::pluginHolderVec.at(var);
            if (plugs->effect == NULL) {
                continue;
            }
            if (plugs->host->shouldDelete)
            {
                MainWindow::pluginHolderVec.remove(var);
                plugs->host->unloadPlugin(plugs->effect);
                delete plugs->host;
                numPlugs--;
                break;
            }
        }
        AudioEngine::shouldDeleteTrack = false;
    }
    if (AudioEngine::requestedPlaybackPos != -1)
    {
        AudioEngine *audio = (AudioEngine*)userData;
        audio->changePlaybackPos();
        customPlayback = true;
        //  AudioManager::requestCustomPlayback();
    }
    for (int var = 0; var < numPlugs ; ++var)
    {

        pluginHolder *plugs = MainWindow::pluginHolderVec.at(var);
        if (plugs->effect == NULL) {
            //   qDebug() <<"Plugin not set in Audiomanager";
            continue;
        }
        if (!plugs->host->canPlay) {
            continue;
        }
        if (isPaused)
        {
            plugs->host->turnOffAllNotes(plugs->effect);
            plugs->host->processAudio(plugs->effect,inputss,outputss,g_blocksize);
        }
        else
        {
            if (customPlayback)
            {
                //plugs->host->setCustomPlackbackPos(AudioManager::requestedPlaybackPos);
            }
            plugs->host->processMidi(plugs->effect);
            plugs->host->processAudio(plugs->effect,inputss,outputss,g_blocksize);
        }
        if(plugs->host->isMuted)
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
    for( i=0; i<framesPerBuffer; i++ )
    {
        *out++ =output_storage[0][i] ;  /* left */
        *out++ =output_storage[1][i]; /* right */
    }

    AudioEngine::silenceChannel(output_storage,num_outputs,g_blocksize);
    return 0;
}

