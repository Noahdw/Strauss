#ifndef AUDIOPLUGINFACTORY_H
#define AUDIOPLUGINFACTORY_H
#include <memory>
#include <QObject>
class AudioPlugin;
class Vst2AudioPlugin;
class TrackMidi;

class AudioPluginFactory
{
public:
    AudioPluginFactory(TrackMidi* track);
    std::unique_ptr<AudioPlugin> createAudioPlugin(QString path);
private:
    std::unique_ptr<AudioPlugin> createVst2Plugin(QString path);
     std::unique_ptr<AudioPlugin> createVst3Plugin(QString path);
    TrackMidi* midiTrack;
};

#endif // AUDIOPLUGINFACTORY_H
