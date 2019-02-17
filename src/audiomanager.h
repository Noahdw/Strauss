#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H
#include <iostream>
#include <cstdint>
#include <fstream>
#include <QObject>
#include <QFile>
#include <src/audioengine.h>
#include "src/mastertrack.h"

class AudioManager
{
public:
    AudioManager(MasterTrack *mTrack);
    int exportAudio(QString filePath);
    void initializePlugins();
    int beginExporting();
    void endExporting();
    void writeHeader(QString filePath);

private:
    void initializeIO();
    float** input_storage= 0;
    float** output_storage = 0;
    float** input = 0;
    float** output = 0;
    int num_outputs = 64;
    int bits_per_sample = 24;
    int num_samples = 0;
    int num_channels = 2;
    std::vector<int> samples;
    MasterTrack* masterTrack;

};

#endif // AUDIOMANAGER_H
