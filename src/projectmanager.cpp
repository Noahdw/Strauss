#include "projectmanager.h"
#include "src/mainwindow.h"
#include "src/common.h"
#include "mastertrack.h"
#include "trackmidi.h"
#include "trackwidget.h"
//./protoc -I=C:/Users/Puter/Documents/MidiInter --cpp_out=./ C:/Users/Puter/Documents/MidiInter/midiinter.proto
ProjectManager::ProjectManager()
{

}

void ProjectManager::saveProject()
{

}

void ProjectManager::saveAsProject(QString path,  MasterTrack *masterTrack)
{

    Application *app = new Application;
    app->set_name(path.toUtf8().constData());
    app->set_blocksize(g_blocksize);
    app->set_sample_rate(g_sampleRate);
    app->set_total_dt(g_quarterNotes * 960);

    for (const auto& track : masterTrack->midiTracks()) {

        auto midi_track = app->add_midi_track();

        midi_track->set_name( track->trackView()->getTrackName().toUtf8().constData());

        auto plugin = midi_track->mutable_master_plugin();
        plugin->set_plugin_url(track->masterPlugin()->pluginPath.toUtf8().constData());
        plugin->set_program_bank(track->masterPlugin()->savePluginState());
        for(auto& item : track->midiData()->noteMap)
        {
            for (uint j = 0; j < item.second.size(); ++j)
            {
                auto midi_data = midi_track->add_midi_data();
                midi_data->set_total_dt(item.first);
                midi_data->set_event(item.second.at(j));
            }

        }

    }

    std::fstream output(path.toUtf8().constData(),std::ios::out | std::ios::trunc | std::ios::binary);
    if (!app->SerializePartialToOstream(&output))
    {
        qDebug() << "Failed to write to protobuf:Application";
    }
    delete app;
    google::protobuf::ShutdownProtobufLibrary();
}

void ProjectManager::loadProject(QString path, MainWindow * main_window,MasterTrack *masterTrack)
{
    std::fstream input(path.toUtf8().constData(), std::ios::in | std::ios::binary);
    if (!input) {
        qDebug() << "Failed to load project file";
        return;
    }

    Application *app = new Application;
    if (!app->ParseFromIstream(&input))
    {
        qDebug() << "Failed to parse project file";
        delete app;
        return;
    }
    for(auto& t : masterTrack->midiTracks())
    {
        masterTrack->removeTrack(t.get());
    }
    g_quarterNotes = app->total_dt() / 960;
    g_totalDt = app->total_dt();
    main_window->audio_engine->changeBlockSize(g_blocksize,app->blocksize());
    main_window->masterTrack->addMidiTrackFromProject(app->midi_track());

    delete app;
    google::protobuf::ShutdownProtobufLibrary();
}
