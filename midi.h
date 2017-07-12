#ifndef MIDI_H
#define MIDI_H
#include <QList>
#include "tracks.h"
class Midi
{
public:
    Midi();

    QList<Tracks> tracks;
    int format;
    int trackChunks;
    bool divisionFormat;
    int ticksPerQuarterNote;
    int deltaTimeSMTPE;
    int framesPerSecondSMTPE;

};


#endif // MIDI_H
