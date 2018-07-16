#include "src/audiomanager.h"
#include "SDK/portaudio.h"

PaStream *stream;
paTestData data;
int AudioManager::requestedPlaybackPos;
bool isPaused = false;
uint AudioManager::blocksize = 256;

float** inputss = 0;
float** outputss = 0;

float** inputStorage= 0;
float** outputStorage = 0;
int numOutputs = 64;
AudioManager::AudioManager()
{
    //QObject::connect(this,&AudioManager::requestCustomPlayback,this,&AudioManager::changePlayback);
}

void AudioManager::startPortAudio()
{
    PaError err = Pa_Initialize();
    if( err != paNoError )
    {
        qDebug() << (  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
        return;
    }
    isRunning = true;
    initializeIO();
    AudioManager::silenceChannel(outputss,numOutputs,blocksize);
    AudioManager::silenceChannel(outputStorage,numOutputs,blocksize);
}

void AudioManager::openStream()
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
                                         blocksize,        // frames per buffer
                                         &patestCallback, /* this is your callback function */
                                         this ); /*This is a pointer that will be passed to
               your callback*/
    if( err != paNoError ) {
        qDebug() << (  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    }
}
void AudioManager::initializeIO() {
    // inputs and outputs are assumed to be float** and are declared elsewhere,
    // most likely the are fields owned by this class. numChannels and blocksize
    // are also fields, both should be size_t (or unsigned int, if you prefer).
    inputss = (float**)malloc(sizeof(float**) * numOutputs);
    outputss = (float**)malloc(sizeof(float**) * numOutputs);
    for(int channel = 0; channel < numOutputs; channel++)
    {
        inputss[channel] = (float*)malloc(sizeof(float*) * blocksize);
        outputss[channel] = (float*)malloc(sizeof(float*) * blocksize);
    }

    inputStorage = (float**)malloc(sizeof(float**) * numOutputs);
    outputStorage = (float**)malloc(sizeof(float**) * numOutputs);
    for(int channel = 0; channel < numOutputs; channel++)
    {
        inputStorage[channel] = (float*)malloc(sizeof(float*) * blocksize);
        outputStorage[channel] = (float*)malloc(sizeof(float*) * blocksize);
    }
}

void AudioManager::requestPlaybackRestart()
{
    for (int var = 0; var < MainWindow::pluginHolderVec.length() ; ++var)
    {
        pluginHolder* plugs=  MainWindow::pluginHolderVec.at(var);
        plugs->host->restartPlayback();
    }
}

void AudioManager::requestPauseOrResume(bool isResume)
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

void AudioManager::changePlaybackPos()
{
    int numPlugs = MainWindow::pluginHolderVec.length();
    for (int var = 0; var < numPlugs ; ++var)
    {

        pluginHolder* plugs=  MainWindow::pluginHolderVec.at(var);
        if (plugs->effect ==NULL) {
            //   qDebug() <<"Plugin not set in Audiomanager";
            continue;
        }
        QObject::connect(this,&AudioManager::changePlaybackPosSignal,plugs->host,&Vst2HostCallback::setCustomPlackbackPos);
        emit changePlaybackPosSignal(requestedPlaybackPos);
    }
}

void AudioManager::silenceChannel(float **channelData, int numChannels, long numFrames) {
    for(int channel = 0; channel < numChannels; ++channel) {
        for(long frame = 0; frame < numFrames; ++frame) {
            channelData[channel][frame] = 0.0f;
        }
    }
}
void AudioManager::startStream()
{
    PaError err = Pa_StartStream( stream );
    if( err != paNoError )
    {
        qDebug() << (  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    }
    requestPlaybackRestart();
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
    if (AudioManager::requestedPlaybackPos != -1)
    {
        AudioManager *audio = (AudioManager*)userData;
        audio->changePlaybackPos();
        customPlayback = true;
        //  AudioManager::requestCustomPlayback();
    }
    for (int var = 0; var < numPlugs ; ++var)
    {

        pluginHolder *plugs = MainWindow::pluginHolderVec.at(var);
        if (plugs->effect ==NULL) {
            //   qDebug() <<"Plugin not set in Audiomanager";
            continue;
        }
        if (!plugs->host->canPlay) {
            continue;
        }
        if (isPaused)
        {
            plugs->host->turnOffAllNotes(plugs->effect);
            plugs->host->processAudio(plugs->effect,inputss,outputss,AudioManager::blocksize);
        }
        else{
            if (customPlayback)
            {
                //plugs->host->setCustomPlackbackPos(AudioManager::requestedPlaybackPos);
            }
            plugs->host->processMidi(plugs->effect);
            plugs->host->processAudio(plugs->effect,inputss,outputss,AudioManager::blocksize);
        }
        if(plugs->host->isMuted)
        {
            AudioManager::silenceChannel(outputss,numOutputs,AudioManager::blocksize);
        }
        else
        {
            for (int i = 0; i < AudioManager::blocksize; ++i)
            {
                outputStorage[0][i] += outputss[0][i];
                outputStorage[1][i] += outputss[1][i];
            }
            AudioManager::silenceChannel(outputss,numOutputs,AudioManager::blocksize);
        }
    }
    if (customPlayback)
    {
        AudioManager::requestedPlaybackPos = -1;
    }
    isPaused = false;
    for( i=0; i<framesPerBuffer; i++ )
    {
        *out++ =outputStorage[0][i] ;  /* left */
        *out++ =outputStorage[1][i]; /* right */
    }

    AudioManager::silenceChannel(outputStorage,numOutputs,AudioManager::blocksize);
    return 0;
}

