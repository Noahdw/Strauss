#include "mainwindow.h"
#include <src/keyboard.h>
#include <windows.h>
#include <src/vst2audioplugin.h>
#include <src/midiplayer.h>
#include <src/common.h>
#include <src/audioengine.h>
#include <src/audiomanager.h>

MidiManager manager;


// init common vars
bool keyboardModeEnabled = false;
double g_tempo = 120;
double g_quarterNotes = 60;
int g_totalDt = g_quarterNotes * 960;
int g_blocksize = 64;
int g_sampleRate = 44100;
double g_volume = 0.666;
QTimeLine *g_timer = new QTimeLine((float)(60.0/(float)g_tempo)*g_quarterNotes*1000);//Song time in ms
QColor g_backgroundColor(82,82,82);
QColor g_baseColor(54,54,54);
QColor g_selectedColor(96,96,96);
//end init

MainWindow::MainWindow(QWidget *parent) :
                                          QMainWindow(parent)
{
    model = new FolderViewAbstractModel(getFoldersFromSettings());
    centralWidget = new QWidget(this);
    masterTrack              = new MasterTrack;
    player = new MidiPlayer(masterTrack);
    pluginEdiorCentralWidget = new QWidget(this);
    stackedCentralWidget = new QStackedWidget;
    audio_engine     = new AudioEngine(masterTrack);



    setCentralWidget(stackedCentralWidget);
    trackScrollArea = new QScrollArea;
    trackScrollArea->setWidgetResizable(true);
    trackScrollArea->setMinimumWidth(1000);
    trackScrollArea->setMinimumHeight(200);
    trackScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    trackScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    trackScrollArea->setAlignment(Qt::AlignTop|Qt::AlignLeft);

    
    folder_view              = new FolderView(model,masterTrack);
    header_container         = new HeaderContainer(audio_engine);
    piano_roll_container     = new PianoRollContainer;
    plugin_editor_container  = new PluginEditorContainer(model,masterTrack);
    track_container          = new TrackContainer(plugin_editor_container,piano_roll_container,masterTrack);
    control_change_container = new ControlChangeContainer(piano_roll_container);
    piano_roll_helper        = new PianoRollHelperView(control_change_container);
    masterTrack->initializeDependencies(track_container,piano_roll_container,plugin_editor_container);
    piano_roll_container->setControlChangeContainer(control_change_container);
    folder_view->pRollContainer = piano_roll_container;
    trackScrollArea->setWidget(track_container);
    centralNotationWidget       = new NotationMainWindow(masterTrack);
    mainLayout                  = new QVBoxLayout;
    QHBoxLayout *helperLayout   = new QHBoxLayout;
    QSplitter   *trackSplitter  = new QSplitter;
    QSplitter   *prollSplitter  = new QSplitter;

    prollSplitter->setOrientation(Qt::Vertical);
    helperLayout->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
    helperLayout->setContentsMargins(0,0,0,0);
    trackSplitter->addWidget(trackScrollArea);
    trackSplitter->addWidget(folder_view);
    mainLayout->addWidget(header_container);
    prollSplitter->addWidget(trackSplitter);
    prollSplitter->addWidget(control_change_container);
    QVBoxLayout *pSpacerLayout = new QVBoxLayout();
    pSpacerLayout->addSpacerItem(new QSpacerItem(0,100,QSizePolicy::Fixed,QSizePolicy::Expanding));
    pSpacerLayout->addWidget(piano_roll_helper);
    helperLayout->addLayout(pSpacerLayout);
    helperLayout->addWidget(prollSplitter);
    mainLayout->addLayout(helperLayout);
    centralWidget->setLayout(mainLayout);
    stackedCentralWidget->addWidget(centralWidget);

    stackedCentralWidget->addWidget(plugin_editor_container);
        stackedCentralWidget->addWidget(centralNotationWidget);
    QObject::connect(track_container,&TrackContainer::switchControlChange,control_change_container,
                     &ControlChangeContainer::switchControlChangeContainer);

    addNewTrack();
    setUpMenuBar();
    AudioEngine::requestedPlaybackPos = -1;
    audio_engine->startPortAudio();
    audio_engine->openStream();
    audio_engine->startStream();

    int devices = player->getDevices();
    for (int i = 0; i < devices; ++i)
    {
        player->openDevice(i);
    }

    QDir dir(QDir::current().path()+"/TempPlugins");
    qDebug() << QDir::current().path()+"/TempPlugins";
    dir.removeRecursively();
    dir.mkdir(QDir::current().path()+"/TempPlugins");
    dir.setPath(QDir::current().path()+"/ProgramBanks");
    if (!dir.exists())
    {
        dir.mkdir(QDir::current().path()+"/ProgramBanks");
    }
}

MainWindow::~MainWindow()
{

}


void MainWindow::on_quitButton_clicked()
{
    midiStreamStop(player->outHandle);

    QCoreApplication::quit();
}


//Opens a midi file and deserializes into a song struct // the song struct is kinda deprecated
void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
                                                    tr("Midi Files (*.mid)"));

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }
        QString s;

        QByteArray array = manager.ReadMidi(file);
        for (int var = 0; var < array.length(); ++var) {
            qint8 val= array.at(var);
            s.append(QString::number(val) + " ");
        }

        manager.song = manager.Deserialize(array);

        file.close();
    }
    track_container->addTrackView(manager.song);
}


void MainWindow::on_PauseButton_clicked(int type)
{
    if (type == -1) {
        player->pausePlayBack();
    }
    else
        player->resumePlayBack();

}

void MainWindow::on_StartButton_clicked()
{
    player->resumePlayBack();
}

void MainWindow::on_actionSave_triggered()
{


}
QFuture<void> future;
bool stopped = false;

// Starts audio engine if a plugin is active
// restarts a song if pressed while playing
void MainWindow::playSong()
{
    audio_engine->requestPlaybackRestart();
}

void MainWindow::saveProject()
{

}

void MainWindow::saveAsProject()
{
    QString filePath = QFileDialog::getSaveFileName(this,tr("Save as"),QDir::currentPath(),tr("format (*.mipr)"));
    ProjectManager project;
    project.saveAsProject(filePath,masterTrack);
}

void MainWindow::loadProject()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open project"), QDir::currentPath(),
                                                    tr("Project files (*.mipr)"));
    ProjectManager project;
    project.loadProject(filePath,this,*track_container);
}

void MainWindow::on_actionPlay_triggered()
{
    playSong();
}

void MainWindow::deleteAllNotes()
{

    centralNotationWidget->addMeasure();
}

void MainWindow::acceptSettingsDialog(int accept)
{
    if (accept == QDialog::Accepted) {
        auto list = getFoldersFromSettings();
        model->setModel(list);
    }
}

void MainWindow::setUpMenuBar()
{

    //Create action crap
    openFileAction = new QAction(tr("&Open Midi File"),this);
    openFileAction->setStatusTip(tr("Open an existing midi file"));
    connect(openFileAction, &QAction::triggered, this, &MainWindow::openFile);

    exportAudioAction = new QAction(tr("&Export Audio"),this);
    exportAudioAction->setStatusTip(tr("Exports as mp3"));
    connect(exportAudioAction, &QAction::triggered, this, &MainWindow::exportAudio);

    saveAction = new QAction("Save project",this);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveProject);

    saveAsAction = new QAction("Save as",this);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAsProject);

    loadAction = new QAction("Load project",this);
    connect(loadAction, &QAction::triggered, this, &MainWindow::loadProject);

    deleteAllNotesAction = new QAction(tr("&Delete all"),this);
    deleteAllNotesAction->setStatusTip(tr("Delete all notes from the roll"));
    connect(deleteAllNotesAction, &QAction::triggered, this, &MainWindow::deleteAllNotes);

    addNewTrackAction = new QAction(tr("&Add Track"),this);
    addNewTrackAction->setStatusTip(tr("Adds a new MIDI track"));
    connect(addNewTrackAction, &QAction::triggered, this, &MainWindow::addNewTrack);

    settingsAction = new QAction(tr("Settings"),this);
    connect(settingsAction, &QAction::triggered, this, &MainWindow::displaySettingsDialog);

    switchNotationAction = new QAction("Notation View");
     connect(switchNotationAction, &QAction::triggered, this, &MainWindow::switchNotationView);

    //Add pause

    //Create menu crap
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(exportAudioAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addAction(loadAction);

    QMenu *toolMenu = menuBar()->addMenu(tr("&Tools"));
    toolMenu->addAction(settingsAction);

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(deleteAllNotesAction);
    editMenu->addAction(addNewTrackAction);

     QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
     viewMenu->addAction(switchNotationAction);
}



void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (keyboardModeEnabled)
    {
        if (event->isAutoRepeat())
        {
            event->ignore();
            QMainWindow::keyReleaseEvent(event);
            return;
        }
        int note = getNoteFromKeyboard(event->key());
        if (note)
        {
            for(const auto& track : masterTrack->midiTracks())
            {
                auto plugin = track->plugin();
                if (track->canRecord())
                {
                    //TODO
                    //plugin.addMidiEvent(0x90,note,velocity,0);
                }

            }
            return;
        }
    }

    switch (event->key())
    {
    case Qt::Key_Alt:
        stackedCentralWidget->setCurrentIndex(!stackedCentralWidget->currentIndex());
        break;
    default:
        break;
    }
    event->ignore();
    QMainWindow::keyPressEvent(event);

}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (keyboardModeEnabled)
    {
        if (event->isAutoRepeat())
        {
            event->ignore();
            QMainWindow::keyReleaseEvent(event);
            return;
        }
        for(const auto& track : masterTrack->midiTracks())
        {
            auto plugin = track->plugin();
            if (track->canRecord())
            {
                int note = getNoteFromKeyboard(event->key());
                if (note)
                {
                    plugin->addMidiEvent(0x90,note,0,0);
                }
            }
        }
    }
    else
    {
        event->ignore();
        QMainWindow::keyReleaseEvent(event);
    }
}

void MainWindow::addNewTrack()
{
   auto track = masterTrack->addTrack();
   centralNotationWidget->addInstrument(track);
}

void MainWindow::displaySettingsDialog()
{
    auto settingsDialog = new SettingsDialog(audio_engine);
    connect(settingsDialog,&SettingsDialog::finished,this,&MainWindow::acceptSettingsDialog);
    settingsDialog->exec();

}

void MainWindow::switchNotationView()
{
    stackedCentralWidget->setCurrentWidget(centralNotationWidget);
}

void MainWindow::exportAudio()
{
    QString filePath = QFileDialog::getSaveFileName(this,tr("Export audio"),QDir::currentPath(),tr("format (*.wav)"));
    audio_engine->stopPortAudio();
    AudioManager audio(masterTrack);
    if (!audio.exportAudio(filePath))
    {
        qDebug() << "Could not export audio";
    }
    audio_engine->startPortAudio();
    audio_engine->openStream();
    audio_engine->startStream();
    // audio_engine->requestPauseOrResume(false);
}

int MainWindow::getNoteFromKeyboard(int key)
{
    switch (key)
    {
    case Qt::Key_A:
        return 60; // Middle C
    case Qt::Key_S:
        return 61;
    case Qt::Key_D:
        return 62;
    case Qt::Key_F:
        return 63;
    case Qt::Key_G:
        return 64;
    case Qt::Key_H:
        return 65;
    case Qt::Key_J:
        return 66;
    case Qt::Key_K:
        return 67;
    case Qt::Key_L:
        return 68;
    case Qt::Key_Semicolon:
        return 69;
    case Qt::Key_Apostrophe:
        return 70;
    case Qt::Key_Return:
        return 71;
    default:
        return 0;
    }
}

QList<QString> MainWindow::getFoldersFromSettings()
{
    QList<QString> list;
    QSettings settings;
    settings.beginGroup("folders");
    int fcount = settings.beginReadArray("paths");
    for (int i = 0; i < fcount; ++i)
    {
        settings.setArrayIndex(i);
        list.append(settings.value("path").toString());
    }
    settings.endArray();
    settings.endGroup();
    return list;
}
