#ifndef MIDIMANAGER_H
#define MIDIMANAGER_H
#include <QFileDialog>

#include <QDebug>
#include <QQmlListProperty>

#include <windows.h>
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
    QQmlListProperty<int> notes();
    MidiManager();
    mSong Deserialize(QByteArray &array);
    QByteArray ReadMidi(QFile &file);
    mTrack track;
    QVector<int> noteVec;
    mSong song;
    static int TPQN;
    QVector<int>* getNoteVecPointer()
    {
        return &noteVec;
    }
    static DWORD statusDWORD(uchar db1, uchar db2, uchar status);
  static  void updateMidiAdd(int note, int veloc, int start, int length, mTrack *track);
   static  void updateMidiDelete(int start, int length, int note, mTrack *track);

signals:
    void notifyTrackViewChanged(mSong *song);

public slots:


};

#endif // MIDIMANAGER_H
