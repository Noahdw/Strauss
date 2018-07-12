#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QObject>
#include <SDK/portaudio.h>
#include <math.h>
#include <src/vst2hostcallback.h>
#include <qdebug.h>
#include <SDK/aeffect.h>
#include <src/mainwindow.h>

struct paTestData
{
    float left_phase;
    float right_phase;
};

class AudioManager : public QObject
{
    Q_OBJECT
public:
    AudioManager();
    void changePlaybackPos();
    void startPortAudio();
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
