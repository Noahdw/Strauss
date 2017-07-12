#include "midi.h"

Midi::Midi()
{

}

QList<Tracks> tracks;
int format;
int trackChunks;
// if bit 15 = 0:
//bits 0-14 = number of delta-time units in each a quarter-note.
//if bit 15 = 1, 0 - 7 = delta-time units per SMTPE frame
// 8 - 14 = form a negative number, representing the number of SMTPE frames per second
//True for 1
bool divisionFormat;

// only used for divisionFormat == true;
int ticksPerQuarterNote;

//Used for divisionFormat == false
int deltaTimeSMTPE;
int framesPerSecondSMTPE;

//This class is basically a song which contains all the midi info
// using a  tracks.cpp list to hold a list of tracks.
