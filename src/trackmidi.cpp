#include "trackmidi.h"

#include "src/trackview.h"
#include "src/pianoroll.h"
#include "src/plugintrackview.h"
#include "src/mainwindow.h"
TrackMidi::TrackMidi()
{
    _midiData.instrumentName = "New Track";
    plugin = new Vst2HostCallback(&_midiData);
    plugin->midiTrack = this;
}

void TrackMidi::setTrackView(TrackView *trackView)
{
    _trackView = trackView;
    //_trackView->pluginTrack->midiTrack = this;
}

void TrackMidi::setPianoRoll(PianoRoll *pianoRoll)
{
    _pianoRoll = pianoRoll;
    plugin->setPianoRollRef(pianoRoll);
}

void TrackMidi::deleteTrack()
{

}

Vst2HostCallback *TrackMidi::loadPlugin(QString actualPath)
{

    if (actualPath == "")
    {
        return NULL;
    }
    QString tempFilePath = QString(QDir::current().path()+"/TempPlugins/%1.dll").arg(FolderView::tempFolderID++);
    if (QFile::exists(tempFilePath))
    {
        QFile::remove(tempFilePath);
    }

    if (!QFile::copy(actualPath, tempFilePath))
    {
        qDebug() << QDir::current().path();
        qDebug() << "Could not copy plugin";
        return NULL;
    }
    int last = actualPath.lastIndexOf("/") + 1;
    QString pluginName = actualPath.right(actualPath.length() - last);

    QByteArray array = tempFilePath.toLocal8Bit();
    char* file = array.data();

    if (plugin->effect == NULL)
    {
        qDebug() << "No plugin is currently set";
        plugin->effect = plugin->loadPlugin(file,pluginName.left(pluginName.size() - 4).toLocal8Bit().data());
        if (plugin->effect == NULL)
        {
            qDebug() << "NULLPTR PLUGIN: in loadPlugin";
            return NULL;
        }
        int state = plugin->configurePluginCallbacks();
        if (state == -1)
        {
            qDebug() << "Failed to configurePluginCallbacks. abort startPlugin";
            delete plugin->effect;
            return NULL;
        }
        trackView()->instrumentLabel->setText(pluginName);

        plugin->startPlugin();
        MainWindow::pluginHolderVec.append(plugin);
        trackView()->pluginTrack->addPlugin(plugin);
        plugin->isMasterPlugin = true;
        plugin->masterPluginTrackView = trackView()->pluginTrack;
        plugin->actual_url = actualPath;

        for (int i = 0; i <  plugin->numParams(plugin->effect); ++i)
        {
            QString name = plugin->getParamName(plugin->effect,i);
            trackView()->comboBox->addItem(name);
        }
        return plugin;
    }
    else{
        Vst2HostCallback *effectPlugin = new Vst2HostCallback;
        effectPlugin->effect = effectPlugin->loadPlugin(file,pluginName.left(pluginName.size() - 4).toLocal8Bit().data());
        if (effectPlugin->effect == NULL)
        {
            qDebug() << "NULLPTR PLUGIN: in loadPlugin";
            return NULL;
        }
        int state = effectPlugin->configurePluginCallbacks();
        if (state == -1)
        {
            qDebug() << "Failed to configurePluginCallbacks. abort startPlugin";
            delete effectPlugin->effect;
            return NULL;
        }

        effectPlugin->startPlugin();
        effectPlugin->isMasterPlugin = false;
        effectPlugin->actual_url = actualPath;
        effectPlugins.append(effectPlugin);
        plugin->masterPluginTrackView->addPlugin(effectPlugin);
        return effectPlugin;
    }
    return NULL;
}



