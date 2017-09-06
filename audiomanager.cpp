#include "audiomanager.h"
#include "SDK/portaudio.h"

PaStream *stream;
paTestData data;
//temp
Vst2HostCallback* masterPlug;
AEffect *masterEffect;
float** inputss = 0;
float** outputss = 0;
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
    silenceChannel(outputss,2,1024);


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
                                blocksize,        /* frames per buffer, i.e. the number
                                                   of sample frames that PortAudio will
                                                   request from the callback. Many apps
                                                   may want to use
                                                   paFramesPerBufferUnspecified, which
                                                   tells PortAudio to pick the best,
                                                   possibly changing, buffer size.*/
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
  inputss = (float**)malloc(sizeof(float**) * 2);
  outputss = (float**)malloc(sizeof(float**) * 2);
  for(int channel = 0; channel < 2; channel++) {
    inputss[channel] = (float*)malloc(sizeof(float*) * blocksize);
    outputss[channel] = (float*)malloc(sizeof(float*) * blocksize);
  }
}
void AudioManager::silenceChannel(float **channelData, int numChannels, long numFrames) {
  for(int channel = 0; channel < numChannels; ++channel) {
    for(long frame = 0; frame < numFrames; ++frame) {
      channelData[channel][frame] = 0.0f;
    }
  }
}
void AudioManager::startStream(Vst2HostCallback *plugin, AEffect *effect)
{
   PaError err = Pa_StartStream( stream );
    if( err != paNoError )
    {
       qDebug() << (  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    }
    masterPlug = plugin;
    masterEffect = effect;

}

int num = 0;
 int patestCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    qDebug() << ++num;

    /* Cast data passed through stream to our structure. */
    paTestData *data = (paTestData*)userData;
    float *out = (float*)outputBuffer;
    unsigned int i;

    (void) inputBuffer; /* Prevent unused variable warning. */

     masterPlug->processAudio(masterEffect,inputss,outputss,1024);
    for( i=0; i<framesPerBuffer; i++ )
    {
        *out++ =outputss[0][i] ;  /* left */
        *out++ =outputss[1][i]; /* right */

    }
    return 0;
}

