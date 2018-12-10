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
struct MidiData
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
    QList<MidiData*> tracks;
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

    MidiData midiData;
    mSong song;
    QVector<int> noteVec;

    static DWORD statusDWORD(uchar db1, uchar db2, uchar status);
    static  void addMidiNote(int note, int veloc, int start, int length, MidiData *midiData);
    static  void addPartialMidiNote(DWORD event, int start, MidiData *midiData);
    static  void removeMidiNote(int start, int length, int note, MidiData *midiData);
    static  void changeMidiVelocity(int start, int note, int velocity, MidiData *midiData);
    static  void recalculateNoteListDT(MidiData *midiData);
    static  int  getVelocityFromNote(int start, int note, MidiData *midiData);

    static int TPQN;


};

#endif // MIDIMANAGER_H
