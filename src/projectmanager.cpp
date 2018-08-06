#include "projectmanager.h"
#include "src/mainwindow.h"
#include "src/common.h"
//./protoc -I=C:/Users/Puter/Documents/MidiInter --cpp_out=./ C:/Users/Puter/Documents/MidiInter/midiinter.proto
ProjectManager::ProjectManager()
{

}

void ProjectManager::saveProject()
{

}

void ProjectManager::saveAsProject(QString path, const TrackContainer &track_container)
{

    Application *app = new Application;
    app->set_name(path.toUtf8().constData());
    app->set_blocksize(g_blocksize);
    app->set_sample_rate(g_sampleRate);
    app->set_total_dt(g_quarterNotes * 960);
    std::vector<const TrackView *> track_views = track_container.getTrackViews();

    for (int i = 0; i < track_views.size(); ++i)
    {
        const TrackView *track_view = track_views.at(i);
        auto midi_track = app->add_midi_track();

        midi_track->set_name(track_view->instrumentName.toUtf8().constData());

        auto plugin = midi_track->mutable_master_plugin();
        plugin->set_plugin_url(track_view->plugin.host->actual_url.toUtf8().constData());
        for(const auto& item : track_view->track->noteMap)
        {
            for (int j = 0; j < item.second.size(); ++j)
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

void ProjectManager::loadProject(QString path, MainWindow * main_window,TrackContainer &track_container)
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
    g_quarterNotes = app->total_dt() / 960;
    main_window->audio_engine->changeBlockSize(g_blocksize,app->blocksize());
    for (int i = 0; i < app->midi_track_size(); ++i)
    {

        auto track_view = track_container.addTrackFromLoadProject(app->midi_track(i),app->total_dt());
        auto master_plugin = app->midi_track(i).master_plugin();
        qDebug() << QString::fromStdString(master_plugin.plugin_url());
        if (master_plugin.IsInitialized())
        {
            track_view->addPluginFromLoadProject( QString::fromStdString(master_plugin.plugin_url()));
        }
    }

    delete app;
    google::protobuf::ShutdownProtobufLibrary();
}
