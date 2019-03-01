#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <math.h>
#include <QObject>
#include <SDK/portaudio.h>
#include <math.h>
#include <src/vst2audioplugin.h>
#include <qdebug.h>
#include <SDK/aeffect.h>
#include "src/mastertrack.h"


class AudioEngine : public QObject
{
    Q_OBJECT
public:
    AudioEngine(MasterTrack *mTrack);
    void changePlaybackPos();
    void startPortAudio();
    void stopPortAudio();
    void openStream();
    void startStream();
    void initializeIO();
    void changeBlockSize(int oldSize, int newSize);
    void requestPlaybackRestart();
    void setPaused(bool paused);
    bool isPaused() {return _paused;}
    bool isRunning = false;
    float sampleRate = 44100;
    MasterTrack *masterTrack;
    static void silenceChannel(float **channelData, int numChannels, long numFrames);
    static uint blocksize;
    static int requestedPlaybackPos;
    static bool shouldDeleteTrack;
    float** input = 0;
    float** output = 0;
    float** input_storage= 0;
    float** output_storage = 0;
    int num_outputs = 64;
private:
    bool _paused = false;
signals:
    void changePlaybackPosSignal( int pos);

};
int patestCallback( const void *inputBuffer, void *outputBuffer,
                    unsigned long framesPerBuffer,
                   const PaStreamCallbackTimeInfo* timeInfo,
                   PaStreamCallbackFlags statusFlags,
                   void *userData );

#endif
