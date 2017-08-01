#ifndef MIDIPLAYER_H
#define MIDIPLAYER_H
#include <midi.h>
#include <windows.h>
#include <mmsystem.h>
#include <QDebug>
#include<midimanager.h>
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
    void Midiman(QString note,QString offOn);
    QVector<int> noteVecInit;
public slots:
    void playNote(QString note,QString onOff);
};

#endif // MIDIPLAYER_H
