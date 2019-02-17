#ifndef MIDIPLAYER_H
#define MIDIPLAYER_H

#include <windows.h>
#include <mmsystem.h>
#include <QDebug>
#include <cstdint>
#include <QtConcurrent/QtConcurrent>
#include <src/midimanager.h>
#include "src/mastertrack.h"
#pragma comment (lib, "winmm.lib")

class MidiPlayer : public QObject
{
     Q_OBJECT
public:
    MidiPlayer(MasterTrack* mTrack);
    void playMidiFile(MidiManager *song);
    void pausePlayBack();
    void resumePlayBack();
    void Midiman(int note,bool active);
    void openDevice(uint deviceNumber);
    int getDevices();
    QVector<int> noteVecInit;
    HMIDISTRM outHandle;
    bool shouldBreak;
    bool needBreak;

    void addMidiAfterRecording();
    static bool canRecordInput;
    static bool recordingOverwrites;
private:
    MasterTrack* masterTrack;

public slots:
    void playNote(int note,bool active);
};

#endif // MIDIPLAYER_H
