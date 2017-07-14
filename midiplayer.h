#ifndef MIDIPLAYER_H
#define MIDIPLAYER_H
#include <midi.h>
#include <windows.h>
#include <mmsystem.h>
#include <QDebug>
#include<midimanager.h>
#pragma comment (lib, "winmm.lib")

class MidiPlayer
{
public:
    MidiPlayer();
    void playMidiFile(MidiManager *song);
    void pausePlayBack();
    void resumePlayBack();

};

#endif // MIDIPLAYER_H
