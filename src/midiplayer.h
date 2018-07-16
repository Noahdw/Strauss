#ifndef MIDIPLAYER_H
#define MIDIPLAYER_H

#include <windows.h>
#include <mmsystem.h>
#include <QDebug>
#include<src/midimanager.h>
#include <cstdint>
#include<QtConcurrent/QtConcurrent>
#pragma comment (lib, "winmm.lib")

class MidiPlayer : public QObject
{
     Q_OBJECT
public:
    MidiPlayer();
    void playMidiFile(MidiManager *song);
    void pausePlayBack();
    void resumePlayBack();
    void Midiman(int note,bool active);
    void openDevice(uint deviceNumber);
    int getDevices();
    QVector<int> noteVecInit;
   // HANDLE hEvent;
    HMIDISTRM outHandle;
    bool shouldBreak;
    bool needBreak;

    static void addMidiAfterRecording();

    static bool canRecordInput;
    static bool recordingOverwrites;
public slots:
    void playNote(int note,bool active);
};

#endif // MIDIPLAYER_H
