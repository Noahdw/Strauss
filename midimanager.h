#ifndef MIDIMANAGER_H
#define MIDIMANAGER_H
#include <QFileDialog>
#include <QFile>
#include <tracks.h>
#include <midi.h>
#include <QDebug>
struct mEvent
{

    int deltaTime;
    unsigned char dataByte1;
   unsigned char velocity;
   unsigned char channel;
  unsigned  char status;
  unsigned  char dataByte2;
    int noteOffOn;
    QString type;

};
struct mTrack
{
    int length;
    QList<mEvent> events;
    QString trackName;
    QString instrumentName;
};
struct mSong
{
    QList<mTrack> tracks;
    int format;
    int trackChunks;
    bool divisionFormat;
    int ticksPerQuarterNote;

    int deltaTimeSMTPE;
    int framesPerSecondSMTPE;
};

class MidiManager
{
public:
    MidiManager();
     mSong Deserialize(QByteArray &array);
     QByteArray ReadMidi(QFile &file);
     mTrack track;
     mSong song;
};

#endif // MIDIMANAGER_H
