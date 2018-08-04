#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <QObject>
#include <SDK/portaudio.h>
#include <math.h>
#include <src/vst2hostcallback.h>
#include <qdebug.h>
#include <SDK/aeffect.h>



class AudioEngine : public QObject
{
    Q_OBJECT
public:
    AudioEngine();
    void changePlaybackPos();
    void startPortAudio();
    void stopPortAudio();
    void openStream();
    void startStream();
    void initializeIO();
    void requestPlaybackRestart();
    void requestPauseOrResume(bool isResume);

    QVector<pluginHolder*> *plugins;
    bool isRunning = false;
    float sampleRate = 44100;

    static void silenceChannel(float **channelData, int numChannels, long numFrames);
    static uint blocksize;
    static int requestedPlaybackPos;

signals:
    void changePlaybackPosSignal( int pos);

};
int patestCallback( const void *inputBuffer, void *outputBuffer,
                    unsigned long framesPerBuffer,
                    const PaStreamCallbackTimeInfo* timeInfo,
                    PaStreamCallbackFlags statusFlags,
                    void *userData );

#endif
