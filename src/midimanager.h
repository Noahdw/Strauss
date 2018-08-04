#ifndef MIDIMANAGER_H
#define MIDIMANAGER_H
#include <QFileDialog>

#include <QDebug>

#include <string>
#include <windows.h>
#include <map>
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
    QVector<int> listOfNotes;
    std::map<int,std::vector<int>> noteMap;
    int totalDT = 0;
};
struct mSong
{
    QList<mTrack*> tracks;
    int format;
    int trackChunks;
    bool divisionFormat;
    int ticksPerQuarterNote;
    int deltaTimeSMTPE;
    int framesPerSecondSMTPE;
};

class MidiManager : public QObject
{
    Q_OBJECT

public:

    MidiManager();
    mSong Deserialize(QByteArray &array);
    QByteArray ReadMidi(QFile &file);
    QVector<int>* getNoteVecPointer()
    {
        return &noteVec;
    }

    mTrack track;
    mSong song;
    QVector<int> noteVec;

    static DWORD statusDWORD(uchar db1, uchar db2, uchar status);
    static  void addMidiNote(int note, int veloc, int start, int length, mTrack *track);
    static  void addPartialMidiNote(DWORD event, int start, mTrack *track);
    static  void removeMidiNote(int start, int length, int note, mTrack *track);
    static  void changeMidiVelocity(int start, int note, int velocity, mTrack *track);
    static  void recalculateNoteListDT(mTrack *track);
    static  int  getVelocityFromNote(int start, int note, mTrack *track);

    static int TPQN;


};

#endif // MIDIMANAGER_H
