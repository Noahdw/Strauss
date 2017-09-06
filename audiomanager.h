#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <SDK/portaudio.h>
#include <math.h>
#include <vst2hostcallback.h>
#include <qdebug.h>
#include <SDK/aeffect.h>
struct paTestData
{
    float left_phase;
    float right_phase;
};


class AudioManager
{
public:
    AudioManager();
    void startPortAudio();
    void openStream();
    void startStream(Vst2HostCallback *plugin,AEffect*effect);
    void silenceChannel(float **channelData, int numChannels, long numFrames);
    void initializeIO();
    bool isRunning = false;
    uint blocksize =0;
    float sampleRate =0;


};
int patestCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData );
#endif // AUDIOMANAGER_H
