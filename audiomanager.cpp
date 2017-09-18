#include "audiomanager.h"
#include "SDK/portaudio.h"

PaStream *stream;
paTestData data;
//temp


float** inputss = 0;
float** outputss = 0;

float** inputStorage= 0;
float** outputStorage = 0;
int numOutputs = 64;
AudioManager::AudioManager()
{

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
                                         &data ); /*This is a pointer that will be passed to
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
    for(int channel = 0; channel < numOutputs; channel++) {
        inputss[channel] = (float*)malloc(sizeof(float*) * blocksize);
        outputss[channel] = (float*)malloc(sizeof(float*) * blocksize);
    }

    inputStorage = (float**)malloc(sizeof(float**) * numOutputs);
    outputStorage = (float**)malloc(sizeof(float**) * numOutputs);
    for(int channel = 0; channel < numOutputs; channel++) {
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
    //hey lazy bum make blocksize static var

    paTestData *data = (paTestData*)userData;
    float *out = (float*)outputBuffer;
    unsigned int i;
    int numPlugs =MainWindow::pluginHolderVec.length();
    (void) inputBuffer; /* Prevent unused variable warning. */
    for (int var = 0; var < numPlugs ; ++var)
    {

        pluginHolder* plugs=  MainWindow::pluginHolderVec.at(var);
        if (plugs->effect ==NULL) {
            //   qDebug() <<"Plugin not set in Audiomanager";
            continue;
        }
        if (!plugs->host->canPlay) {
            continue;
        }
        plugs->host->processMidi(plugs->effect);
        plugs->host->processAudio(plugs->effect,inputss,outputss,256);

        for (int i = 0; i < 256; ++i) {
            outputStorage[0][i] += outputss[0][i];
            outputStorage[1][i] += outputss[1][i];

        }
        AudioManager::silenceChannel(outputss,numOutputs,256);
    }


    for( i=0; i<framesPerBuffer; i++ )
    {
        //  qDebug() << outputss[0][i];
        *out++ =outputStorage[0][i] ;  /* left */
        *out++ =outputStorage[1][i]; /* right */

    }

    AudioManager::silenceChannel(outputStorage,numOutputs,256);
    return 0;
}

