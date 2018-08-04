#include "src/audioengine.h"
#include "SDK/portaudio.h"
#include <src/mainwindow.h>
PaStream *stream;

int AudioEngine::requestedPlaybackPos;
bool isPaused = false;


float** inputss = 0;
float** outputss = 0;

float** inputStorage= 0;
float** outputStorage = 0;
int numOutputs = 64;
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
    AudioEngine::silenceChannel(outputss,numOutputs,g_blocksize);
    AudioEngine::silenceChannel(outputStorage,numOutputs,g_blocksize);
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
    inputss = (float**)malloc(sizeof(float**) * numOutputs);
    outputss = (float**)malloc(sizeof(float**) * numOutputs);
    for(int channel = 0; channel < numOutputs; channel++)
    {
        inputss[channel] = (float*)malloc(sizeof(float*) * g_blocksize);
        outputss[channel] = (float*)malloc(sizeof(float*) * g_blocksize);
    }

    inputStorage = (float**)malloc(sizeof(float**) * numOutputs);
    outputStorage = (float**)malloc(sizeof(float**) * numOutputs);
    for(int channel = 0; channel < numOutputs; channel++)
    {
        inputStorage[channel] = (float*)malloc(sizeof(float*) * g_blocksize);
        outputStorage[channel] = (float*)malloc(sizeof(float*) * g_blocksize);
    }
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
            AudioEngine::silenceChannel(outputss,numOutputs,g_blocksize);
        }
        else
        {
            for (int i = 0; i < g_blocksize; ++i)
            {
                outputStorage[0][i] += outputss[0][i] * g_volume;
                outputStorage[1][i] += outputss[1][i] * g_volume;
            }
            AudioEngine::silenceChannel(outputss,numOutputs,g_blocksize);
        }
    }
    if (customPlayback)
    {
        AudioEngine::requestedPlaybackPos = -1;
    }
    isPaused = false;
    for( i=0; i<framesPerBuffer; i++ )
    {
        *out++ =outputStorage[0][i] ;  /* left */
        *out++ =outputStorage[1][i]; /* right */
    }

    AudioEngine::silenceChannel(outputStorage,numOutputs,g_blocksize);
    return 0;
}

