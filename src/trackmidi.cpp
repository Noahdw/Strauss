#include "trackmidi.h"

#include "src/trackview.h"
#include "src/pianoroll.h"
#include "src/plugintrackview.h"
#include "src/mainwindow.h"
#include "src/mastertrack.h"

TrackMidi::TrackMidi(MasterTrack *mTrack) : masterTrack(mTrack)
{
    _midiData.instrumentName = "New Track";
    _plugin = new Vst2HostCallback(&_midiData);
    _plugin->midiTrack = this;
}

TrackMidi::~TrackMidi()
{
    masterTrack->removeTrack(this);
    _trackView->deleteLater();
    _pianoRoll->deleteLater();
}

void TrackMidi::setTrackView(TrackView *trackView)
{
    _trackView = trackView;
    //_trackView->pluginTrack->midiTrack = this;
}

void TrackMidi::setPianoRoll(PianoRoll *pianoRoll)
{
    _pianoRoll = pianoRoll;
    _plugin->setPianoRollRef(pianoRoll);
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

    if (_plugin->effect == NULL)
    {
        qDebug() << "No plugin is currently set";
        _plugin->effect = _plugin->loadPlugin(file,pluginName.left(pluginName.size() - 4).toLocal8Bit().data());
        if (_plugin->effect == NULL)
        {
            qDebug() << "NULLPTR PLUGIN: in loadPlugin";
            return NULL;
        }
        int state = _plugin->configurePluginCallbacks();
        if (state == -1)
        {
            qDebug() << "Failed to configurePluginCallbacks. abort startPlugin";
            delete _plugin->effect;
            return NULL;
        }
        trackView()->instrumentLabel->setText(pluginName);

        _plugin->startPlugin();
        trackView()->pluginTrack->addPlugin(_plugin);
        _plugin->isMasterPlugin = true;
        _plugin->masterPluginTrackView = trackView()->pluginTrack;
        _plugin->actual_url = actualPath;

        for (int i = 0; i <  _plugin->numParams(_plugin->effect); ++i)
        {
            QString name = _plugin->getParamName(_plugin->effect,i);
            trackView()->comboBox->addItem(name);
        }
        return _plugin;
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
        _plugin->masterPluginTrackView->addPlugin(effectPlugin);
        return effectPlugin;
    }
    return NULL;
}



