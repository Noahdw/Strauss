#include "audiopluginfactory.h"
#include <QDir>
#include "folderview.h"
AudioPluginFactory::AudioPluginFactory(TrackMidi *track) : midiTrack(track)
{

}

std::unique_ptr<AudioPlugin> AudioPluginFactory::createAudioPlugin(QString path)
{
    auto type = path.section('.',-1);
    if (type == "") return nullptr;
    if(type == ".dll")
        return createVst2Plugin(path);
    return nullptr;
}

std::unique_ptr<AudioPlugin> AudioPluginFactory::createVst2Plugin(QString path)
{
    QString tempFilePath = QString(QDir::current().path()+"/TempPlugins/%1.dll").arg(FolderView::tempFolderID++);
    if (QFile::exists(tempFilePath))
    {
        QFile::remove(tempFilePath);
    }

    if (!QFile::copy(path, tempFilePath))
    {
        qDebug() << QDir::current().path();
        qDebug() << "Could not copy plugin";
        return false;
    }
    int last = path.lastIndexOf("/") + 1;
    QString pluginName = path.right(path.length() - last);

    std::unique_ptr<AudioPlugin> plugin = std::make_unique<Vst2AudioPlugin>(midiTrack);
    if(plugin->loadPlugin(tempFilePath,pluginName))
        return plugin;

    return nullptr;
}
