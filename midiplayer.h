#ifndef MIDIPLAYER_H
#define MIDIPLAYER_H
#include <midi.h>
#include <windows.h>
#include <mmsystem.h>
#include <QDebug>
#pragma comment (lib, "winmm.lib")

class MidiPlayer
{
public:
    MidiPlayer();
    void playMidiFile(Midi *song);

};

#endif // MIDIPLAYER_H
