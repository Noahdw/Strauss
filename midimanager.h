#ifndef MIDIMANAGER_H
#define MIDIMANAGER_H
#include <QFileDialog>
#include <QFile>
#include <tracks.h>
#include <midi.h>
#include <QDebug>
class MidiManager
{
public:
    MidiManager();
     Midi Deserialize(QByteArray &array);
     QByteArray ReadMidi(QFile &file);
};

#endif // MIDIMANAGER_H
